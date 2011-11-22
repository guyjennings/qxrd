#include "qxrdacquisition.h"
#include "qxrdmutexlocker.h"
#include "qxrddataprocessor.h"
#include "qxrdallocator.h"
#include "qxrdacquiredialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdwindow.h"
#include "qxrdapplication.h"

#include <QThreadPool>
#include <QtConcurrentRun>

QxrdAcquisition::QxrdAcquisition(QxrdDocument *doc, QxrdDataProcessor *proc, QxrdAllocator *allocator)
  : QxrdAcquisitionOperations(doc, proc, allocator),
    m_AcquiredImages("acquired"),
    m_ControlPanel(NULL),
    m_Idling(1)
{
  if (qcepDebug(DEBUG_APP)) {
    g_Application->printMessage("QxrdAcquisition::QxrdAcquisition");
  }

  m_SynchronizedAcquisition = new QxrdSynchronizedAcquisition(this);

  connect(prop_ExposureTime(), SIGNAL(valueChanged(double,int)), this, SLOT(onExposureTimeChanged(double)));
  connect(prop_BinningMode(), SIGNAL(valueChanged(int,int)), this, SLOT(onBinningModeChanged(int)));
  connect(prop_CameraGain(), SIGNAL(valueChanged(int,int)), this, SLOT(onCameraGainChanged(int)));

  if (sizeof(void*) == 4) {
    connect(prop_TotalBufferSizeMB32(), SIGNAL(valueChanged(int,int)), this, SLOT(onBufferSizeChanged(int)));
    onBufferSizeChanged(get_TotalBufferSizeMB32());
  } else {
    connect(prop_TotalBufferSizeMB64(), SIGNAL(valueChanged(int,int)), this, SLOT(onBufferSizeChanged(int)));
    onBufferSizeChanged(get_TotalBufferSizeMB64());
  }

  connect(&m_Watcher, SIGNAL(finished()), this, SLOT(onAcquireComplete()));
  connect(&m_IdleTimer, SIGNAL(timeout()), this, SLOT(onIdleTimeout()));

  m_IdleTimer.start(1000);
}

QxrdAcquisition::~QxrdAcquisition()
{
  if (qcepDebug(DEBUG_APP)) {
    g_Application->printMessage("QxrdAcquisition::~QxrdAcquisition");
  }
}

void QxrdAcquisition::initialize()
{
}

void QxrdAcquisition::shutdown()
{
  if (qcepDebug(DEBUG_APP)) {
    g_Application->printMessage("QxrdAcquisition::shutdown()");
  }

  shutdownAcquisition();

  thread()->exit();
}

void QxrdAcquisition::onBufferSizeChanged(int newMB)
{
  m_Allocator -> changedSizeMB(newMB);
}

void QxrdAcquisition::acquire()
{
  THREAD_CHECK;

  if (m_Acquiring.tryLock()) {
    set_Cancelling(false);
    set_Triggered(false);

    g_Application->statusMessage("Starting acquisition");
    emit acquireStarted();

    QxrdAcquisitionParameterPack params(get_FilePattern(),
                                        get_ExposureTime(),
                                        get_SummedExposures(),
                                        get_PreTriggerFiles(),
                                        get_PostTriggerFiles(),
                                        get_PhasesInGroup(),
                                        get_SkippedExposuresAtStart(),
                                        get_SkippedExposures());

    QFuture<void> res = QtConcurrent::run(this, &QxrdAcquisition::doAcquire, params);

    m_Watcher.setFuture(res);
  } else {
    g_Application->statusMessage("Acquisition is already in progress");
  }
}

QxrdAcquisition::QxrdAcquisitionParameterPack QxrdAcquisition::acquisitionParameterPack()
{
  return QxrdAcquisitionParameterPack (get_FilePattern(),
                                       get_ExposureTime(),
                                       get_SummedExposures(),
                                       get_PreTriggerFiles(),
                                       get_PostTriggerFiles(),
                                       get_PhasesInGroup(),
                                       get_SkippedExposuresAtStart(),
                                       get_SkippedExposures());
}

QxrdAcquisition::QxrdDarkAcquisitionParameterPack QxrdAcquisition::darkAcquisitionParameterPack()
{
  return QxrdDarkAcquisitionParameterPack(get_FilePattern(),
                                          get_ExposureTime(),
                                          get_DarkSummedExposures(),
                                          get_SkippedExposuresAtStart());

}

void QxrdAcquisition::acquireDark()
{
  THREAD_CHECK;

  if (m_Acquiring.tryLock()) {
    set_Cancelling(false);
    set_Triggered(true);

    g_Application->statusMessage("Starting dark acquisition");
    emit acquireStarted();

    QxrdDarkAcquisitionParameterPack params(get_FilePattern(),
                                            get_ExposureTime(),
                                            get_DarkSummedExposures(),
                                            get_SkippedExposuresAtStart());

    QFuture<void> res = QtConcurrent::run(this, &QxrdAcquisition::doAcquireDark, params);

    m_Watcher.setFuture(res);
  } else {
    g_Application->statusMessage("Acquisition is already in progress");
  }
}

void QxrdAcquisition::onAcquireComplete()
{
  m_Acquiring.unlock();

  emit acquireComplete();
}

void QxrdAcquisition::cancel()
{
  set_Cancelling(true);

  m_NAcquiredImages.release(1);
}

void QxrdAcquisition::trigger()
{
  set_Triggered(true);
}

void QxrdAcquisition::clearDropped()
{
  prop_DroppedFrames() -> setValue(0);
}

int  QxrdAcquisition::acquisitionStatus(double time)
{
  if (m_Acquiring.tryLock()) {
    m_Acquiring.unlock();

    //    printf("m_Acquiring.tryLock() succeeded\n");

    return 1;
  }

  QMutex mutex;
  QxrdMutexLocker lock(__FILE__, __LINE__, &mutex);

  if (m_StatusWaiting.wait(&mutex, (int)(time*1000))) {
    //    printf("m_StatusWaiting.wait succeeded\n");
    return 1;
  } else {
    //    printf("m_StatusWaiting.wait failed\n");
    return 0;
  }
}

void QxrdAcquisition::indicateDroppedFrame(int n)
{
  QString msg = tr("Frame %1 dropped [%2/%3 MB Used]")
                  .arg(n)
                  .arg(m_Allocator->allocatedMemoryMB())
                  .arg(m_Allocator->maximumMemoryMB());

  g_Application->statusMessage(msg);
  g_Application->printMessage(msg);

  prop_DroppedFrames() -> incValue(1);
}

template <typename T>
void QxrdAcquisition::accumulateAcquiredImage(QSharedPointer< QxrdImageData<T> > image, QxrdInt32ImageDataPtr accum, QxrdMaskDataPtr overflow)
{
  if (image && accum && overflow) {
    long nPixels = get_NRows()*get_NCols();
    int ovflwlvl = get_OverflowLevel();
    T* src = image->data();
    quint32* dst = accum->data();
    short int* ovf = overflow->data();
    int nsummed = accum->get_SummedExposures();

    if (nsummed == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        g_Application->printMessage(tr("Frame %1 saved").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        T val = *src++;

        if (val>ovflwlvl) {
          *ovf++ = 1;
        } else {
          *ovf++ = 0;
        }

        *dst++ = val;
      }
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        g_Application->printMessage(tr("Frame %1 summed").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        T val = *src++;

        if (val>ovflwlvl) {
          *ovf++ |= 1;
        } else {
          ovf++;
        }

        *dst++ += val;
      }
    }

    accum -> prop_SummedExposures() -> incValue(1);
  }
}

void QxrdAcquisition::getFileBaseAndName(QString filePattern, int fileIndex, int phase, int nphases, QString &fileBase, QString &fileName)
{
  int width = get_FileIndexWidth();

  if (nphases == 0) {
    fileBase = filePattern+tr("-%1.dark.tif").arg(fileIndex,width,10,QChar('0'));
    fileName = QDir(m_DataProcessor -> darkOutputDirectory()).filePath(fileBase);
  } else {
    if (nphases > 1) {
      int phswidth = get_FilePhaseWidth();
      fileBase = filePattern+tr("-%1-%2.tif").arg(fileIndex,width,10,QChar('0')).arg(phase,phswidth,10,QChar('0'));
    } else {
      fileBase = filePattern+tr("-%1.tif").arg(fileIndex,width,10,QChar('0'));
    }
    fileName = QDir(m_DataProcessor -> rawOutputDirectory()).filePath(fileBase);
  }
}

void QxrdAcquisition::processImage(QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
{
  if (image) {
    int w=image->get_Width(), h=image->get_Height();

    QxrdInt32ImageDataPtr proc = m_Allocator->newInt32Image(QxrdAllocator::AllocateFromReserve, w,h);
    QxrdMaskDataPtr ovf = m_Allocator->newMask(QxrdAllocator::AllocateFromReserve, w,h, 0);

    if (proc == NULL || ovf == NULL) {
      indicateDroppedFrame(0);
      return;
    }

    proc->copyFrom(image);
    overflow->copyMaskTo(ovf);

    if (qcepDebug(DEBUG_ACQUIRE)) {
      g_Application->printMessage(tr("processAcquiredImage(%1,%2) %3 summed exposures")
                        .arg(fileIndex).arg(phase).arg(image->get_SummedExposures()));
    }

    QString fileName;
    QString fileBase;

    getFileBaseAndName(filePattern, fileIndex, phase, nPhases, fileBase, fileName);

    if (qcepDebug(DEBUG_ACQUIRE)) {
      g_Application->printMessage(tr("Fn: %1, Fi: %2, Phs: %3")
                        .arg(fileName).arg(fileIndex).arg(phase));
    }

    set_FileBase(fileBase);
    //        m_DataProcessor -> set_FileName(fileName);

    QFileInfo finfo(fileName);

    proc -> set_QxrdVersion(get_QxrdVersion());
    proc -> set_QtVersion(get_QtVersion());
    proc -> set_FileBase(fileBase);
    proc -> set_FileName(fileName);
    proc -> set_Title(finfo.fileName());
    proc -> set_ExposureTime(get_ExposureTime());
    proc -> set_DateTime(QDateTime::currentDateTime());
    proc -> set_HBinning(1);
    proc -> set_VBinning(1);
    proc -> set_CameraGain(get_CameraGain());
    proc -> set_DataType(QxrdInt32ImageData::Raw32Data);
    proc -> set_UserComment1(get_UserComment1());
    proc -> set_UserComment2(get_UserComment2());
    proc -> set_UserComment3(get_UserComment3());
    proc -> set_UserComment4(get_UserComment4());
    proc -> set_ImageSaved(false);
    proc -> set_Triggered(trig);

    copyDynamicProperties(proc.data());

    if (nPhases == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        g_Application->printMessage(tr("32 bit Dark Image acquired"));
      }

      proc -> set_ImageNumber(-1);
      proc -> set_PhaseNumber(-1);
      proc -> set_NPhases(0);
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        g_Application->printMessage(tr("32 bit Image %1 acquired").arg(get_FileIndex()));
      }

      proc -> set_ImageNumber(fileIndex);
      proc -> set_PhaseNumber(phase);
      proc -> set_NPhases(nPhases);
    }

    m_DataProcessor -> acquiredInt32Image(proc, ovf);
  }
}

void QxrdAcquisition::processImage(const QxrdProcessArgs &args)
{
  QThread::currentThread()->setObjectName("processImage");

  g_Application->printMessage(tr("QxrdAcquisition::processImage %1 %2 start").arg(args.m_FilePattern).arg(args.m_FileIndex));

  processImage(args.m_FilePattern, args.m_FileIndex, args.m_Phase, args.m_NPhases, args.m_Trig, args.m_Image, args.m_Overflow);

  g_Application->printMessage(tr("QxrdAcquisition::processImage %1 %2 end").arg(args.m_FilePattern).arg(args.m_FileIndex));
}

void QxrdAcquisition::processAcquiredImage(QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
{
//  printf("processAcquiredImage(""%s"",%d,%d,img,ovf)\n", qPrintable(filePattern), fileIndex, phase);

//  processImage(filePattern, fileIndex, phase, nPhases, trig, image, overflow);

  QtConcurrent::run(this, &QxrdAcquisition::processImage,
                    QxrdProcessArgs(filePattern, fileIndex, phase, nPhases, trig, image, overflow));
}

void QxrdAcquisition::processDarkImage(QString filePattern, int fileIndex, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
{
//  printf("processDarkImage(""%s"",%d,img,ovf)\n", qPrintable(filePattern), fileIndex);

  processImage(filePattern, fileIndex, -1, 0, true, image, overflow);
}

//void QxrdAcquisition::acquisitionError(int n)
//{
//  cancel();

//  emit criticalMessage(tr("Acquisition Error %1").arg(n));
//}

void QxrdAcquisition::acquisitionError(const char *fn, int ln, int n)
{
  cancel();

  g_Application->criticalMessage(tr("Acquisition Error %1 at line %2 in file %3").arg(n).arg(ln).arg(fn));
}

QxrdAcquireDialogBase *QxrdAcquisition::controlPanel(QxrdWindow *win)
{
  if (win) {
    m_Window = win;

    m_ControlPanel = new QxrdAcquireDialog(m_Document, m_Window, this, m_DataProcessor, m_Window);

    return m_ControlPanel;
  } else {
    return NULL;
  }
}

void QxrdAcquisition::setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin)
{
  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition -> setNIDAQPlugin(nidaqPlugin);
  }
}

QxrdNIDAQPluginInterface *QxrdAcquisition::nidaqPlugin() const
{
  if (m_SynchronizedAcquisition) {
    return m_SynchronizedAcquisition -> nidaqPlugin();
  } else {
    return NULL;
  }
}

QxrdSynchronizedAcquisition* QxrdAcquisition::synchronizedAcquisition() const
{
  return m_SynchronizedAcquisition;
}

int QxrdAcquisition::cancelling()
{
  int res = get_Cancelling();

  if (res) {
    g_Application->printMessage(tr("Cancelling acquisition"));
    g_Application->statusMessage(tr("Cancelling acquisition"));
  }

  return res;
}

void QxrdAcquisition::doAcquire(QxrdAcquisitionParameterPack parms)
{
  QTime acqTimer;
  acqTimer.start();

  QThread::currentThread()->setObjectName("doAcquire");

  stopIdling();

  QString fileBase = parms.fileBase();
  int fileIndex = get_FileIndex();
  double exposure = parms.exposure();
  int nsummed = parms.nsummed();
  int preTrigger = parms.preTrigger();
  int postTrigger = parms.postTrigger();
  int nphases = parms.nphases();
  int skipBefore = parms.skipBefore();

  if (skipBefore <= 0) {
    if (get_LastAcquired() != 1) {
      skipBefore = 1;
    }
  }

  set_LastAcquired(1);

  int skipBetween = parms.skipBetween();
  int nPreTriggered = 0;

  if (preTrigger <= 0) {
    set_Triggered(true);
  }

  if (synchronizedAcquisition()) {
    synchronizedAcquisition()->prepareForAcquisition(&parms);
  }

  QVector<QVector<QxrdInt32ImageDataPtr> >res(nphases);
  QVector<QVector<QxrdMaskDataPtr> >      ovf(nphases);

  g_Application->printMessage("Starting acquisition");

  for (int p=0; p<nphases; p++) {
    res[p].resize(preTrigger+1);
    ovf[p].resize(preTrigger+1);

    for (int t=0; t<=preTrigger; t++) {
      res[p][t] = m_Allocator->newInt32Image(QxrdAllocator::AllocateFromReserve,
                                             get_NCols(), get_NRows());
      ovf[p][t] = m_Allocator->newMask(QxrdAllocator::AllocateFromReserve,
                                       get_NCols(), get_NRows(), 0);

      if (res[p][t]==NULL || ovf[p][t]==NULL) {
        g_Application->criticalMessage("Insufficient memory for acquisition operation");
        goto cancel;
      }
    }
  }

  for (int i=0; i<skipBefore; i++) {
    if (cancelling()) goto cancel;
    g_Application->printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
    acquireFrame(exposure);
  }

  for (int i=0; i<postTrigger; i += (get_Triggered() ? 1:0)) {
    if (cancelling()) goto cancel;

    for (int p=0; p<nphases; p++) {
      QString fb, fn;

      QxrdInt32ImageDataPtr nres = m_Allocator->newInt32Image(QxrdAllocator::AllocateFromReserve,
                                                              get_NCols(), get_NRows());
      QxrdMaskDataPtr novf = m_Allocator->newMask(QxrdAllocator::AllocateFromReserve,
                                                   get_NCols(), get_NRows(), 0);
      res[p][0] = nres;
      ovf[p][0] = novf;

      if (nres == NULL || novf == NULL) {
        indicateDroppedFrame(0);
      }
//      res[p][0] -> clear();
//      ovf[p][0] -> clear();

      if (nres) res[p][0] -> set_SummedExposures(0);
      if (novf) ovf[p][0] -> set_SummedExposures(0);

      getFileBaseAndName(fileBase, fileIndex+i, p, nphases, fb, fn);

      if (nres) res[p][0] -> set_FileBase(fb);
      if (nres) res[p][0] -> set_FileName(fn);
    }

    if (qcepDebug(DEBUG_ACQUIRETIME)) {
      g_Application->printMessage(tr("Clearing took %1 msec").arg(acqTimer.restart()));
    }

    if (i != 0) {
      for (int k=0; k<skipBetween; k++) {
        if (cancelling()) goto cancel;
        g_Application->printMessage(tr("Skipping %1 of %2").arg(k+1).arg(skipBetween));
        acquireFrame(exposure);

        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          g_Application->printMessage(tr("Frame after %1 msec").arg(acqTimer.restart()));
        }
      }
    }

    if (cancelling()) goto cancel;

    for (int s=0; s<nsummed; s++) {
      for (int p=0; p<nphases; p++) {
        if (res[p][0]) {
          emit acquiredFrame(res[p][0]->get_FileBase(), fileIndex+i, p, nphases, s, nsummed, i, postTrigger);
        }

        QxrdInt16ImageDataPtr img = acquireFrame(exposure);

        if (img && res[p][0] && ovf[p][0]) {
          accumulateAcquiredImage(img, res[p][0], ovf[p][0]);
        } else if (!cancelling()){
          indicateDroppedFrame(0);
        }

        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          g_Application->printMessage(tr("accumulateAcquiredImage %1 msec idx:%2 post:%3 sum: %4 ph:%5")
                                       .arg(acqTimer.restart())
                                       .arg(fileIndex)
                                       .arg(i)
                                       .arg(s)
                                       .arg(p)
                                       );
        }

        if (cancelling()) goto saveCancel;
      }
    }

  saveCancel:
    if (get_Triggered()) {
      int nPre = qMin(preTrigger, nPreTriggered);

      for (int ii=nPre; ii >= 1; ii--) {
        for (int p=0; p<nphases; p++) {
          processAcquiredImage(fileBase, fileIndex, p, nphases, false, res[p][ii], ovf[p][ii]);

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            g_Application->printMessage(tr("processAcquiredImage(line %1) %2 msec idx:%3 pre:%4 ph:%5")
                                        .arg(__LINE__)
                                        .arg(acqTimer.restart())
                                        .arg(fileIndex)
                                        .arg(ii)
                                        .arg(p)
                                        );
          }

          res[p].pop_back();
          ovf[p].pop_back();
        }
        fileIndex++;
        set_FileIndex(fileIndex);
      }

      nPreTriggered = 0;

      for (int p=0; p<nphases; p++) {
        if (res[p][0] && ovf[p][0]) {
          processAcquiredImage(fileBase, fileIndex, p, nphases, true, res[p][0], ovf[p][0]);
        }

        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          g_Application->printMessage(tr("processAcquiredImage(line %1) %2 msec idx:%3 pre:%4 ph:%5")
                                      .arg(__LINE__)
                                      .arg(acqTimer.restart())
                                      .arg(fileIndex)
                                      .arg(i)
                                      .arg(p)
                                      );
        }

      }
      fileIndex++;
      set_FileIndex(fileIndex);
    } else {
      nPreTriggered++;
      for (int p=0; p<nphases; p++) {
        res[p].push_front(res[p].last());
        ovf[p].push_front(ovf[p].last());
        res[p].pop_back();
        ovf[p].pop_back();
      }
    }
  }

  g_Application->statusMessage(tr("Acquisition complete"));
  g_Application->printMessage(tr("Acquisition complete"));

cancel:
  if (synchronizedAcquisition()) {
    synchronizedAcquisition()->finishedAcquisition();
  }

  startIdling();
}

void QxrdAcquisition::doAcquireDark(QxrdDarkAcquisitionParameterPack parms)
{
  QThread::currentThread()->setObjectName("doAcquireDark");

  stopIdling();

  QString fileBase = parms.fileBase();
  int fileIndex = get_FileIndex();
  double exposure = parms.exposure();
  int nsummed = parms.nsummed();
  int skipBefore = parms.skipBefore();

  if (skipBefore <= 0) {
    if (get_LastAcquired() != -1) {
      skipBefore = 1;
    }
  }

  set_LastAcquired(-1);

  g_Application->printMessage("Starting dark acquisition");

  if (synchronizedAcquisition()) {
    synchronizedAcquisition()->prepareForDarkAcquisition(&parms);
  }

  QxrdInt32ImageDataPtr res = m_Allocator->newInt32Image(QxrdAllocator::AllocateFromReserve,
                                                         get_NCols(), get_NRows());
  QxrdMaskDataPtr overflow  = m_Allocator->newMask(QxrdAllocator::AllocateFromReserve,
                                                   get_NCols(), get_NRows(),0);
  QString fb, fn;

  if (res == NULL || overflow == NULL) {
    g_Application->criticalMessage("Insufficient memory for acquisition operation");
    goto cancel;
  }

  getFileBaseAndName(fileBase, fileIndex, -1, 1, fb, fn);

  res -> set_FileBase(fb);
  res -> set_FileName(fn);

  for (int i=0; i<skipBefore; i++) {
    if (cancelling()) goto cancel;
    g_Application->printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
    acquireFrame(exposure);
  }

  for (int i=0; i<nsummed; i++) {
    if (cancelling()) goto cancel;

    emit acquiredFrame(res->get_FileBase(), fileIndex, 0, 1, i, nsummed, 0, 1);

    QxrdInt16ImageDataPtr img = acquireFrame(exposure);

    if (img) {
      accumulateAcquiredImage(img, res, overflow);
    } else {
      if (!cancelling()){
        indicateDroppedFrame(0);
      } else {
        goto cancel;
      }
    }
  }

saveCancel:
  processDarkImage(fileBase, fileIndex, res, overflow);

  g_Application->statusMessage(tr("Acquisition complete"));
  g_Application->printMessage(tr("Acquisition complete"));

cancel:
  if (synchronizedAcquisition()) {
    synchronizedAcquisition()->finishedAcquisition();
  }

  startIdling();
}

void QxrdAcquisition::stopIdling()
{
  m_Idling.fetchAndStoreOrdered(0);
  flushImageQueue();
  beginAcquisition();
}

void QxrdAcquisition::startIdling()
{
  m_Idling.fetchAndStoreOrdered(1);
  endAcquisition();
}

void QxrdAcquisition::onIdleTimeout()
{
  if (m_Idling) {
    QxrdInt16ImageDataPtr res = acquireFrameIfAvailable(get_ExposureTime());

    if (res) {
      m_DataProcessor->idleInt16Image(res);
    }
  }
}

void QxrdAcquisition::flushImageQueue()
{
  int n = m_NAcquiredImages.available();
  m_NAcquiredImages.acquire(n);

  for (int i=0; i<n; i++) {
    m_AcquiredImages.dequeue();
  }
}

QxrdInt16ImageDataPtr QxrdAcquisition::acquireFrame(double exposure)
{
  if (qcepDebug(DEBUG_ACQUIRE)) {
    g_Application->printMessage(tr("acquireFrame(%1) : allocated %2 MB : %3 images available")
                                .arg(exposure)
                                .arg(m_Allocator->allocatedMemoryMB())
                                .arg(m_NAcquiredImages.available())
                                );
  }

  m_NAcquiredImages.acquire(1);

  QxrdInt16ImageDataPtr res = m_AcquiredImages.dequeue();

  return res;
}

QxrdInt16ImageDataPtr QxrdAcquisition::acquireFrameIfAvailable(double exposure)
{
  if (qcepDebug(DEBUG_ACQUIRE)) {
    g_Application->printMessage(tr("acquireFrameIfAvailable(%1) : allocated %2 MB").arg(exposure).arg(m_Allocator->allocatedMemoryMB()));
  }

  QxrdInt16ImageDataPtr res;

  while (m_NAcquiredImages.available() >= 1) {
    res = acquireFrame(exposure);
  }

  return res;
}

void QxrdAcquisition::enqueueAcquiredFrame(QxrdInt16ImageDataPtr img)
{
  m_AcquiredImages.enqueue(img);

  m_NAcquiredImages.release(1);
}
