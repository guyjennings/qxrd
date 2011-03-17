#include "qxrdacquisition.h"
#include "qxrdmutexlocker.h"
#include "qxrddataprocessor.h"
#include "qxrdallocator.h"
#include "qxrdacquiredialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdwindow.h"

#include <QThreadPool>
#include <QtConcurrentRun>

QxrdAcquisition::QxrdAcquisition(QxrdDataProcessor *proc, QxrdAllocator *allocator)
  : QxrdAcquisitionOperations(proc, allocator),
    m_AcquiredImages("acquired"),
    m_ControlPanel(NULL),
    m_Idling(1)
{
  m_SynchronizedAcquisition = new QxrdSynchronizedAcquisition(this);

  connect(prop_ExposureTime(), SIGNAL(changedValue(double)), this, SLOT(onExposureTimeChanged(double)));
  connect(prop_BinningMode(), SIGNAL(changedValue(int)), this, SLOT(onBinningModeChanged(int)));
  connect(prop_CameraGain(), SIGNAL(changedValue(int)), this, SLOT(onCameraGainChanged(int)));

  if (sizeof(void*) == 4) {
    connect(prop_TotalBufferSizeMB32(), SIGNAL(changedValue(int)), this, SLOT(onBufferSizeChanged(int)));
    onBufferSizeChanged(get_TotalBufferSizeMB32());
  } else {
    connect(prop_TotalBufferSizeMB64(), SIGNAL(changedValue(int)), this, SLOT(onBufferSizeChanged(int)));
    onBufferSizeChanged(get_TotalBufferSizeMB64());
  }

  connect(&m_Watcher, SIGNAL(finished()), this, SLOT(onAcquireComplete()));
  connect(&m_IdleTimer, SIGNAL(timeout()), this, SLOT(onIdleTimeout()));

  m_IdleTimer.start(1000);
}

QxrdAcquisition::~QxrdAcquisition()
{
//  if (qcepDebug(DEBUG_ACQUIRE)) {
//             printf("QxrdAcquisition::~QxrdAcquisition\n");
//  }
}

void QxrdAcquisition::initialize()
{
}

void QxrdAcquisition::shutdown()
{
  shutdownAcquisition();

  thread()->exit();
}

void QxrdAcquisition::onBufferSizeChanged(int newMB)
{
  m_Allocator -> changedSizeMB(newMB);
}

void QxrdAcquisition::allocateMemoryForAcquisition()
{
  THREAD_CHECK;

  m_Allocator -> dimension(get_NCols(), get_NRows());
}

void QxrdAcquisition::acquire()
{
  THREAD_CHECK;

  if (m_Acquiring.tryLock()) {
    set_Cancelling(false);

    emit statusMessage("Starting acquisition");
    emit acquireStarted();

    QxrdAcquisitionParameterPack params(get_FilePattern(),
                                        get_ExposureTime(),
                                        get_SummedExposures(),
                                        get_PreTriggerFiles(),
                                        get_PostTriggerFiles(),
                                        get_PhasesInGroup(),
                                        get_SkippedExposuresAtStart(),
                                        get_SkippedExposures());

    typedef void (QxrdAcquisition::*MFType)(QxrdAcquisitionParameterPack);
    MFType p = &QxrdAcquisition::doAcquire;
    QFuture<void> res = QtConcurrent::run(this, p, params);

    m_Watcher.setFuture(res);
  } else {
    emit statusMessage("Acquisition is already in progress");
  }
}

void QxrdAcquisition::acquireDark()
{
  THREAD_CHECK;

  if (m_Acquiring.tryLock()) {
    set_Cancelling(false);

    emit statusMessage("Starting dark acquisition");
    emit acquireStarted();

    QxrdDarkAcquisitionParameterPack params(get_FilePattern(),
                                            get_ExposureTime(),
                                            get_DarkSummedExposures(),
                                            get_SkippedExposuresAtStart());

    typedef void (QxrdAcquisition::*MFType)(QxrdDarkAcquisitionParameterPack);
    MFType p = &QxrdAcquisition::doAcquireDark;
    QFuture<void> res = QtConcurrent::run(this, p, params);

    m_Watcher.setFuture(res);
  } else {
    emit statusMessage("Acquisition is already in progress");
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

  emit statusMessage(msg);
  emit printMessage(msg);

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
        emit printMessage(tr("Frame %1 saved").arg(nsummed));
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
        emit printMessage(tr("Frame %1 summed").arg(nsummed));
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
  if (nphases == 0) {
    fileBase = filePattern+tr("-%1.dark.tif").arg(fileIndex,5,10,QChar('0'));
    fileName = QDir(m_DataProcessor -> darkOutputDirectory()).filePath(fileBase);
  } else {
    if (nphases > 1) {
      fileBase = filePattern+tr("-%1-%2.tif").arg(fileIndex,5,10,QChar('0')).arg(phase,3,10,QChar('0'));
    } else {
      fileBase = filePattern+tr("-%1.tif").arg(fileIndex,5,10,QChar('0'));
    }
    fileName = QDir(m_DataProcessor -> rawOutputDirectory()).filePath(fileBase);
  }
}

void QxrdAcquisition::processImage(QString filePattern, int fileIndex, int phase, int nPhases, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
{
  if (image) {
    QxrdInt32ImageDataPtr proc = m_Allocator->newInt32Image(QxrdAllocator::AllocateFromReserve);
    QxrdMaskDataPtr ovf = m_Allocator->newMask(QxrdAllocator::AllocateFromReserve, 0);

    if (proc == NULL || ovf == NULL) {
      indicateDroppedFrame(0);
      return;
    }

    proc->copyFrom(image);
    overflow->copyMaskTo(ovf);

    if (qcepDebug(DEBUG_ACQUIRE)) {
      emit printMessage(tr("processAcquiredImage(%1,%2) %3 summed exposures")
                        .arg(fileIndex).arg(phase).arg(image->get_SummedExposures()));
    }

    QString fileName;
    QString fileBase;

    getFileBaseAndName(filePattern, fileIndex, phase, nPhases, fileBase, fileName);

    if (qcepDebug(DEBUG_ACQUIRE)) {
      emit printMessage(tr("Fn: %1, Fi: %2, Phs: %3")
                        .arg(fileName).arg(get_FileIndex()).arg(phase));
    }

    set_FileBase(fileBase);
    //        m_DataProcessor -> set_FileName(fileName);

    QFileInfo finfo(fileName);

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

    copyDynamicProperties(proc.data());

    if (nPhases == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        emit printMessage(tr("32 bit Dark Image acquired"));
      }

      proc -> set_ImageNumber(-1);
      proc -> set_PhaseNumber(-1);
      proc -> set_NPhases(0);
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        emit printMessage(tr("32 bit Image %1 acquired").arg(get_FileIndex()));
      }

      proc -> set_ImageNumber(fileIndex);
      proc -> set_PhaseNumber(phase);
      proc -> set_NPhases(nPhases);
    }

    m_DataProcessor -> acquiredInt32Image(proc, ovf);
  }
}

void QxrdAcquisition::processAcquiredImage(QString filePattern, int fileIndex, int phase, int nPhases, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
{
//  printf("processAcquiredImage(""%s"",%d,%d,img,ovf)\n", qPrintable(filePattern), fileIndex, phase);

  processImage(filePattern, fileIndex, phase, nPhases, image, overflow);
}

void QxrdAcquisition::processDarkImage(QString filePattern, int fileIndex, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
{
//  printf("processDarkImage(""%s"",%d,img,ovf)\n", qPrintable(filePattern), fileIndex);

  processImage(filePattern, fileIndex, -1, 0, image, overflow);
}

void QxrdAcquisition::acquisitionError(int n)
{
  cancel();

  emit criticalMessage(tr("Acquisition Error %1").arg(n));
}

void QxrdAcquisition::acquisitionError(int ln, int n)
{
  cancel();

  emit criticalMessage(tr("Acquisition Error %1 at line %2").arg(n).arg(ln));
}

QxrdAcquireDialog *QxrdAcquisition::controlPanel(QxrdWindow *win)
{
  if (win) {
    m_Window = win;

    m_ControlPanel = new QxrdAcquireDialog(m_Window, this, m_DataProcessor, m_Window);

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
    emit printMessage(tr("Cancelling acquisition"));
    emit statusMessage(tr("Cancelling acquisition"));
  }

  return res;
}

void QxrdAcquisition::doAcquire(QxrdAcquisitionParameterPack parms)
{
  stopIdling();

  QString fileBase = parms.fileBase();
  int fileIndex = get_FileIndex();
  double exposure = parms.exposure();
  int nsummed = parms.nsummed();
  int preTrigger = parms.preTrigger();
  int postTrigger = parms.postTrigger();
  int nphases = parms.nphases();
  int skipBefore = parms.skipBefore();
  int skipBetween = parms.skipBetween();

  if (synchronizedAcquisition()) {
    synchronizedAcquisition()->prepareForAcquisition(&parms);
  }

  QVector<QxrdInt32ImageDataPtr> res(nphases);
  QVector<QxrdMaskDataPtr>       ovf(nphases);

  emit printMessage("Starting acquisition");

  for (int p=0; p<nphases; p++) {
    res[p] = m_Allocator->newInt32Image(QxrdAllocator::AllocateFromReserve);
    ovf[p] = m_Allocator->newMask(QxrdAllocator::AllocateFromReserve,0);

    if (res[p]==NULL || ovf[p]==NULL) {
      emit criticalMessage("Insufficient memory for acquisition operation");
      goto cancel;
    }
  }

  for (int i=0; i<skipBefore; i++) {
    if (cancelling()) goto cancel;
    emit statusMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
    acquireFrame(exposure);
  }

  for (int i=0; i<postTrigger; i++) {
    if (cancelling()) goto cancel;

    set_FileIndex(fileIndex+i);

    for (int p=0; p<nphases; p++) {
      QString fb, fn;

      res[p]->clear();
      ovf[p]->clear();

      getFileBaseAndName(fileBase, fileIndex+i, p, nphases, fb, fn);

      if (res[p]) {
        res[p] -> set_FileBase(fb);
        res[p] -> set_FileName(fn);
      }
    }

    if (i != 0) {
      for (int k=0; k<skipBetween; k++) {
        emit statusMessage(tr("Skipping %1 of %2").arg(k+1).arg(skipBetween));
        acquireFrame(exposure);
      }
    }

    if (cancelling()) goto cancel;

    for (int s=0; s<nsummed; s++) {
      for (int p=0; p<nphases; p++) {
        if (res[p]) {
          emit acquiredFrame(res[p]->get_FileBase(), fileIndex+i, p, nphases, s, nsummed, i, postTrigger);
        }

        QxrdInt16ImageDataPtr img = acquireFrame(exposure);

        if (img) {
          accumulateAcquiredImage(img, res[p], ovf[p]);
        } else {
          indicateDroppedFrame(0);
        }

        if (cancelling()) goto saveCancel;
      }
    }

  saveCancel:
    prop_FileIndex()->incValue(1);

    for (int p=0; p<nphases; p++) {
      processAcquiredImage(fileBase, fileIndex+i, p, nphases, res[p], ovf[p]);
    }
  }

  emit statusMessage(tr("Acquisition complete"));
  emit printMessage(tr("Acquisition complete"));

cancel:
  if (synchronizedAcquisition()) {
    synchronizedAcquisition()->finishedAcquisition();
  }

  startIdling();
}

void QxrdAcquisition::doAcquireDark(QxrdDarkAcquisitionParameterPack parms)
{
  stopIdling();

  QString fileBase = parms.fileBase();
  int fileIndex = get_FileIndex();
  double exposure = parms.exposure();
  int nsummed = parms.nsummed();
  int skipBefore = parms.skipBefore();

  emit printMessage("Starting dark acquisition");

  if (synchronizedAcquisition()) {
    synchronizedAcquisition()->prepareForDarkAcquisition(&parms);
  }

  QxrdInt32ImageDataPtr res = m_Allocator->newInt32Image(QxrdAllocator::AllocateFromReserve);
  QxrdMaskDataPtr overflow  = m_Allocator->newMask(QxrdAllocator::AllocateFromReserve,0);
  QString fb, fn;

  if (res == NULL || overflow == NULL) {
    emit criticalMessage("Insufficient memory for acquisition operation");
    goto cancel;
  }

  getFileBaseAndName(fileBase, fileIndex, -1, 1, fb, fn);

  res -> set_FileBase(fb);
  res -> set_FileName(fn);

  for (int i=0; i<skipBefore; i++) {
    if (cancelling()) goto cancel;
    acquireFrame(exposure);
  }

  for (int i=0; i<nsummed; i++) {
    if (cancelling()) goto saveCancel;

    emit acquiredFrame(res->get_FileBase(), fileIndex, 0, 1, i, nsummed, 0, 1);

    QxrdInt16ImageDataPtr img = acquireFrame(exposure);

    if (img) {
      accumulateAcquiredImage(img, res, overflow);
    } else {
      indicateDroppedFrame(0);
    }
  }

saveCancel:
  processDarkImage(fileBase, fileIndex, res, overflow);

  emit statusMessage(tr("Acquisition complete"));
  emit printMessage(tr("Acquisition complete"));

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
  m_NAcquiredImages.acquire(1);

  QxrdInt16ImageDataPtr res = m_AcquiredImages.dequeue();

  return res;
}

QxrdInt16ImageDataPtr QxrdAcquisition::acquireFrameIfAvailable(double exposure)
{
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
