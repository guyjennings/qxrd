#include "qxrddebug.h"
#include "qxrdacquisition.h"
#include "qxrdmutexlocker.h"
#include "qxrddataprocessor.h"
#include "qxrdallocator.h"
#include "qxrdacquisitiondialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdwindow.h"
#include "qxrdapplication.h"

#include <QThreadPool>
#include <QtConcurrentRun>
#include <QDir>
#include <QMetaProperty>

QxrdAcquisition::QxrdAcquisition(QxrdSettingsSaverWPtr saver,
                                 QxrdExperimentWPtr doc,
                                 QxrdDataProcessorWPtr proc,
                                 QxrdAllocatorWPtr allocator)
  : QObject(),
    m_ObjectNamer(this, "acquisition"),
    m_Saver(saver),
    m_QxrdVersion(QxrdSettingsSaverPtr(), this,"qxrdVersion",STR(QXRD_VERSION), "QXRD Version Number"),
    m_QtVersion(QxrdSettingsSaverPtr(), this,"qtVersion",qVersion(), "QT Version Number"),
//    m_DetectorType(QxrdSettingsSaverPtr(), this, "detectorType", 0, "Detector Type (0 = simulated, 1 = PE, 2 = Pilatus, 3 = EPICS, 4 = Files)"),
    m_ExposureTime(saver, this,"exposureTime",0.1, "Exposure Time (in sec)"),
    m_SkippedExposuresAtStart(saver, this,"skippedExposuresAtStart",0, "Exposures to Skip at Start"),
    m_LastAcquired(QxrdSettingsSaverPtr(), this, "lastAcquired", 0, "Internal Acquisition Flag"),
    m_PhasesInGroup(saver, this,"phasesInGroup",1, "Number of Image Phases"),
    m_SummedExposures(saver, this,"summedExposures",1, "Summed Exposures per Image"),
    m_SkippedExposures(saver, this,"skippedExposures",0, "Skipped Exposures between Images"),
    m_PreTriggerFiles(saver, this,"preTriggerFiles",0, "Number of pre-Trigger Images"),
    m_PostTriggerFiles(saver, this,"postTriggerFiles",1, "Number of post-Trigger Images"),
    m_FileIndex(saver, this,"fileIndex",0, "File Index"),
    m_FilePattern(saver, this,"filePattern","", "File Name Pattern"),
    m_FileIndexWidth(saver, this, "fileIndexWidth", 5, "Digits in File Index Field"),
    m_FilePhaseWidth(saver, this, "filePhaseWidth", 3, "Digits in Phase Number Field"),
    m_FileOverflowWidth(saver, this, "fileOverflowWidth", 5, "Digits in Overflow Index Field"),
    m_DarkSummedExposures(saver, this,"darkSummedExposures",1, "Summed Exposures in Dark Image"),
    m_CameraGain(saver, this,"cameraGain",0, "Detector Gain"),
    m_BinningMode(saver, this,"binningMode",0, "Binning Mode"),
    m_FileBase(saver, this,"fileBase","", "File Base"),
    m_NRows(saver, this, "nRows", 2048, "Number of Rows"),
    m_NCols(saver, this, "nCols", 2048, "Number of Cols"),
    m_OverflowLevel(saver, this, "overflowLevel", 65500, "Overflow level (per exposure)"),
    m_AcquireDark(QxrdSettingsSaverPtr(), this, "acquireDark", 0, "Acquire Dark Image?"),
    m_Cancelling(QxrdSettingsSaverPtr(), this, "cancelling", 0, "Cancel Acquisition?"),
    m_Triggered(QxrdSettingsSaverPtr(), this, "triggered", 0, "Trigger Acquisition"),
    m_Raw16SaveTime(saver, this,"raw16SaveTime", 0.1, "Time to save 16 bit images"),
    m_Raw32SaveTime(saver, this,"raw32SaveTime", 0.2, "Time to save 32 bit images"),
    m_RawSaveTime(saver, this,"rawSaveTime", 0.2, "Time to save raw images"),
    m_DarkSaveTime(saver, this,"darkSaveTime", 0.2, "Time to save dark images"),
    m_UserComment1(saver, this,"userComment1","", "User Comment 1"),
    m_UserComment2(saver, this,"userComment2","", "User Comment 2"),
    m_UserComment3(saver, this,"userComment3","", "User Comment 3"),
    m_UserComment4(saver, this,"userComment4","", "User Comment 4"),
    m_Normalization(saver, this, "normalization", QcepDoubleList(), "Normalization Values"),
    m_DroppedFrames(QxrdSettingsSaverPtr(), this,"droppedFrames",0, "Number of Dropped Frames"),
    m_LiveViewAtIdle(saver, this, "liveViewAtIdle", false, "Live View during Idle"),
    m_AcquisitionCancelsLiveView(saver, this, "acquisitionCancelsLiveView", true, "Acquisition operations cancel live view"),
    m_Mutex(QMutex::Recursive),
    m_SynchronizedAcquisition(NULL),
    m_AcquisitionExtraInputs(NULL),
    m_Experiment(doc),
    m_Window(),
    m_Allocator(allocator),
    m_DataProcessor(proc),
    m_Detector(),
    m_AcquiredImages("acquired"),
    m_ControlPanel(NULL),
    m_Idling(1)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisition::QxrdAcquisition(%p)\n", this);
  }
}

void QxrdAcquisition::initialize()
{
  connect(prop_Raw16SaveTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateSaveTimes()));
  connect(prop_Raw32SaveTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateSaveTimes()));
  connect(prop_SummedExposures(), SIGNAL(valueChanged(int,int)), this, SLOT(updateSaveTimes()));
  connect(prop_DarkSummedExposures(), SIGNAL(valueChanged(int,int)), this, SLOT(updateSaveTimes()));

//  m_FileIndex.setDebug(1);

  QxrdAllocatorPtr alloc(m_Allocator);

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdAcquisition::QxrdAcquisition");
  }

  m_SynchronizedAcquisition = QxrdSynchronizedAcquisitionPtr(
        new QxrdSynchronizedAcquisition(m_Saver, this));

  m_AcquisitionExtraInputs = QxrdAcquisitionExtraInputsPtr(new QxrdAcquisitionExtraInputs(m_Saver, m_Experiment, this));
  m_AcquisitionExtraInputs -> initialize();

  connect(prop_ExposureTime(), SIGNAL(valueChanged(double,int)), this, SLOT(onExposureTimeChanged()));
  connect(prop_BinningMode(), SIGNAL(valueChanged(int,int)), this, SLOT(onBinningModeChanged()));
  connect(prop_CameraGain(), SIGNAL(valueChanged(int,int)), this, SLOT(onCameraGainChanged()));

  if (alloc) {
    if (sizeof(void*) == 4) {
      connect(alloc->prop_TotalBufferSizeMB32(), SIGNAL(valueChanged(int,int)), this, SLOT(onBufferSizeChanged(int)));
      onBufferSizeChanged(alloc->get_TotalBufferSizeMB32());
    } else {
      connect(alloc->prop_TotalBufferSizeMB64(), SIGNAL(valueChanged(int,int)), this, SLOT(onBufferSizeChanged(int)));
      onBufferSizeChanged(alloc->get_TotalBufferSizeMB64());
    }
  }

  connect(&m_Watcher, SIGNAL(finished()), this, SLOT(onAcquireComplete()));
  connect(&m_IdleTimer, SIGNAL(timeout()), this, SLOT(onIdleTimeout()));

  m_IdleTimer.start(1000);
}

QxrdAcquisition::~QxrdAcquisition()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisition::~QxrdAcquisition(%p)\n", this);
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdAcquisition::~QxrdAcquisition");
  }
}

void QxrdAcquisition::shutdown()
{
  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdAcquisition::shutdown()");
  }

  shutdownAcquisition();

  thread()->exit();
}

void QxrdAcquisition::updateSaveTimes()
{
  if (get_SummedExposures() <= 1) {
    set_RawSaveTime(get_Raw16SaveTime());
  } else {
    set_RawSaveTime(get_Raw32SaveTime());
  }

  if (get_DarkSummedExposures() <= 1) {
    set_DarkSaveTime(get_Raw16SaveTime());
  } else {
    set_DarkSaveTime(get_Raw32SaveTime());
  }
}

void QxrdAcquisition::dynamicProperties()
{
  QByteArray name;

  if (g_Application) {
    foreach(name, dynamicPropertyNames()) {
      g_Application->printMessage(tr("acquisition.%1\n").arg(name.data()));
    }
  }
}

void QxrdAcquisition::copyDynamicProperties(QObject *dest)
{
  if (dest) {
    QByteArray name;

    foreach(name, dynamicPropertyNames()) {
      dest -> setProperty(name.data(), property(name.data()));
    }
  }
}

void QxrdAcquisition::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition->writeSettings(settings, section+"/sync");
  }

  if (m_AcquisitionExtraInputs) {
    m_AcquisitionExtraInputs->writeSettings(settings, section+"/extrainputs");
  }

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings, true);
}

void QxrdAcquisition::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition->readSettings(settings, section+"/sync");
  }

  if (m_AcquisitionExtraInputs) {
    m_AcquisitionExtraInputs->readSettings(settings, section+"/extrainputs");
  }

  QcepProperty::readSettings(this, &staticMetaObject, section, settings, true);
}

void QxrdAcquisition::setWindow(QxrdWindowWPtr win)
{
  m_Window = win;
}

void QxrdAcquisition::setDetector(QxrdDetectorWPtr det)
{
  m_Detector = det;
}

void QxrdAcquisition::printMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdAcquisition::criticalMessage(QString msg, QDateTime /*ts*/)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->criticalMessage(msg);
  }
}

void QxrdAcquisition::statusMessage(QString msg, QDateTime /*ts*/)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->statusMessage(msg);
  }
}

void QxrdAcquisition::onBufferSizeChanged(int newMB)
{
  QxrdAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    alloc -> changedSizeMB(newMB);
  }
}

QxrdAllocatorWPtr QxrdAcquisition::allocator() const
{
  return m_Allocator;
}

void QxrdAcquisition::acquire()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "acquire", Qt::BlockingQueuedConnection));
  } else {
    if (m_Acquiring.tryLock()) {
      set_Cancelling(false);
      set_Triggered(false);

      statusMessage("Starting acquisition");
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
      statusMessage("Acquisition is already in progress");
    }
  }
}

QxrdAcquisitionParameterPack QxrdAcquisition::acquisitionParameterPack()
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

QxrdDarkAcquisitionParameterPack QxrdAcquisition::darkAcquisitionParameterPack()
{
  return QxrdDarkAcquisitionParameterPack(get_FilePattern(),
                                          get_ExposureTime(),
                                          get_DarkSummedExposures(),
                                          get_SkippedExposuresAtStart());

}

void QxrdAcquisition::acquireDark()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "acquireDark", Qt::BlockingQueuedConnection));
  } else {
    if (m_Acquiring.tryLock()) {
      set_Cancelling(false);
      set_Triggered(true);

      statusMessage("Starting dark acquisition");
      emit acquireStarted();

      QxrdDarkAcquisitionParameterPack params(get_FilePattern(),
                                              get_ExposureTime(),
                                              get_DarkSummedExposures(),
                                              get_SkippedExposuresAtStart());

      QFuture<void> res = QtConcurrent::run(this, &QxrdAcquisition::doAcquireDark, params);

      m_Watcher.setFuture(res);
    } else {
      statusMessage("Acquisition is already in progress");
    }
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
  QxrdAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    QString msg = tr("Frame %1 dropped [%2/%3 MB Used]")
        .arg(n)
        .arg(alloc->allocatedMemoryMB())
        .arg(alloc->maximumMemoryMB());

    statusMessage(msg);
    printMessage(msg);

    prop_DroppedFrames() -> incValue(1);
  }
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
        printMessage(tr("Frame %1 saved").arg(nsummed));
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

      accum->set_Normalization(image->get_Normalization());
      accum->set_ExtraInputs(image->get_ExtraInputs());
      accum->set_SummedExposures(1);
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 summed").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        T val = *src++;

        if (val>ovflwlvl) {
          *ovf++ += 1;
        } else {
          ovf++;
        }

        *dst++ += val;
      }

      accum->prop_Normalization()   -> incValue(image->get_Normalization());
      accum->prop_ExtraInputs()     -> incValue(image->get_ExtraInputs());
      accum->prop_SummedExposures() -> incValue(1);
    }
  }
}

void QxrdAcquisition::getFileBaseAndName(QString filePattern, int fileIndex, int phase, int nphases, QString &fileBase, QString &fileName)
{
  int width = get_FileIndexWidth();

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    if (nphases == 0) {
      fileBase = filePattern+tr("-%1.dark.tif").arg(fileIndex,width,10,QChar('0'));
      fileName = QDir(proc -> darkOutputDirectory()).filePath(fileBase);
    } else {
      if (nphases > 1) {
        int phswidth = get_FilePhaseWidth();
        fileBase = filePattern+tr("-%1-%2.tif").arg(fileIndex,width,10,QChar('0')).arg(phase,phswidth,10,QChar('0'));
      } else {
        fileBase = filePattern+tr("-%1.tif").arg(fileIndex,width,10,QChar('0'));
      }
      fileName = QDir(proc -> rawOutputDirectory()).filePath(fileBase);
    }
  }
}

void QxrdAcquisition::processImage(QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
{
  if (image) {
    int w=image->get_Width(), h=image->get_Height();

    QxrdInt32ImageDataPtr proc = QxrdAllocator::newInt32Image(m_Allocator, QxrdAllocator::AllocateFromReserve, w,h);
    QxrdMaskDataPtr ovf = QxrdAllocator::newMask(m_Allocator, QxrdAllocator::AllocateFromReserve, w,h, 0);

    if (proc == NULL || ovf == NULL) {
      indicateDroppedFrame(0);
      return;
    }

    proc->copyFrom(image);
    overflow->copyMaskTo(ovf);

    if (qcepDebug(DEBUG_ACQUIRE)) {
      printMessage(tr("processAcquiredImage(%1,%2) %3 summed exposures")
                   .arg(fileIndex).arg(phase).arg(image->get_SummedExposures()));
    }

    QString fileName;
    QString fileBase;

    getFileBaseAndName(filePattern, fileIndex, phase, nPhases, fileBase, fileName);

    if (qcepDebug(DEBUG_ACQUIRE)) {
      printMessage(tr("Fn: %1, Fi: %2, Phs: %3")
                   .arg(fileName).arg(fileIndex).arg(phase));
    }

    set_FileBase(fileBase);
    //        m_DataProcessor -> set_FileName(fileName);

    QFileInfo finfo(fileName);

    QDateTime now = QDateTime::currentDateTime();
    double msec = now.toMSecsSinceEpoch()/1000.0;

    proc -> set_QxrdVersion(get_QxrdVersion());
    proc -> set_QtVersion(get_QtVersion());
    proc -> set_FileBase(fileBase);
    proc -> set_FileName(fileName);
    proc -> set_Title(finfo.fileName());
    proc -> set_ExposureTime(get_ExposureTime());
    proc -> set_DateTime(now);
    proc -> set_TimeStamp(msec);
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
    proc -> set_Normalization(get_Normalization());

    copyDynamicProperties(proc.data());

    if (nPhases == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("32 bit Dark Image acquired"));
      }

      proc -> set_ImageNumber(-1);
      proc -> set_PhaseNumber(-1);
      proc -> set_NPhases(0);
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("32 bit Image %1 acquired").arg(get_FileIndex()));
      }

      proc -> set_ImageNumber(fileIndex);
      proc -> set_PhaseNumber(phase);
      proc -> set_NPhases(nPhases);
    }

    QxrdDataProcessorPtr processor(m_DataProcessor);

    if (processor) {
      processor -> acquiredInt32Image(proc, ovf);
    }
  }
}

void QxrdAcquisition::processImage(const QxrdProcessArgs &args)
{
  QThread::currentThread()->setObjectName("processImage");

  printMessage(tr("QxrdAcquisition::processImage %1 %2 start").arg(args.m_FilePattern).arg(args.m_FileIndex));

  processImage(args.m_FilePattern, args.m_FileIndex, args.m_Phase, args.m_NPhases, args.m_Trig, args.m_Image, args.m_Overflow);

  printMessage(tr("QxrdAcquisition::processImage %1 %2 end").arg(args.m_FilePattern).arg(args.m_FileIndex));
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

QxrdAcquisitionDialogPtr QxrdAcquisition::controlPanel(QxrdWindowWPtr win)
{
  if (win) {
    m_Window = win;

    m_ControlPanel = new QxrdAcquisitionDialog(m_Experiment,
                                               m_Window,
                                               this,
                                               m_DataProcessor,
                                               m_Window.data());

    return m_ControlPanel;
  } else {
    return NULL;
  }
}

void QxrdAcquisition::setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin)
{
  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition -> setNIDAQPlugin(nidaqPlugin);
  }

  if (m_AcquisitionExtraInputs) {
    m_AcquisitionExtraInputs -> setNIDAQPlugin(nidaqPlugin);
  }
}

QxrdNIDAQPluginInterfacePtr QxrdAcquisition::nidaqPlugin() const
{
  if (m_SynchronizedAcquisition) {
    return m_SynchronizedAcquisition -> nidaqPlugin();
  } else {
    return QxrdNIDAQPluginInterfacePtr();
  }
}

QxrdSynchronizedAcquisitionPtr QxrdAcquisition::synchronizedAcquisition() const
{
  return m_SynchronizedAcquisition;
}

QxrdAcquisitionExtraInputsPtr QxrdAcquisition::acquisitionExtraInputs() const
{
  return m_AcquisitionExtraInputs;
}

int QxrdAcquisition::cancelling()
{
  int res = get_Cancelling();

  if (res) {
    printMessage(tr("Cancelling acquisition"));
    statusMessage(tr("Cancelling acquisition"));
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

  if (acquisitionExtraInputs()) {
    acquisitionExtraInputs()->prepareForAcquisition(&parms);
  }

  QVector<QVector<QxrdInt32ImageDataPtr> >res(nphases);
  QVector<QVector<QxrdMaskDataPtr> >      ovf(nphases);

  printMessage("Starting acquisition");
  printMessage(tr("Starting acquisition : QxrdAcquisition::doAcquire(fileBase = \"%1\" : index = %2 : Expos = %3 : nsum = %4 : preTrig = %5 : postTrig = %6 : nphases = %7 : skip = %8)")
               .arg(fileBase).arg(fileIndex).arg(exposure).arg(nsummed).arg(preTrigger).arg(postTrigger).arg(nphases).arg(skipBefore));

  for (int p=0; p<nphases; p++) {
    res[p].resize(preTrigger+1);
    ovf[p].resize(preTrigger+1);

    for (int t=0; t<=preTrigger; t++) {
      res[p][t] = QxrdAllocator::newInt32Image(m_Allocator,
                                               QxrdAllocator::AllocateFromReserve,
                                               get_NCols(), get_NRows());
      ovf[p][t] = QxrdAllocator::newMask(m_Allocator,
                                         QxrdAllocator::AllocateFromReserve,
                                         get_NCols(), get_NRows(), 0);

      if (res[p][t]==NULL || ovf[p][t]==NULL) {
        criticalMessage("Insufficient memory for acquisition operation");
        goto cancel;
      }
    }
  }

  for (int i=0; i<skipBefore; i++) {
    if (cancelling()) goto cancel;
    printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
    acquireFrame(exposure);
  }

  for (int i=0; i<postTrigger; i += (get_Triggered() ? 1:0)) {
    if (cancelling()) goto cancel;

    for (int p=0; p<nphases; p++) {
      QString fb, fn;

      QxrdInt32ImageDataPtr nres = QxrdAllocator::newInt32Image(m_Allocator,
                                                                QxrdAllocator::AllocateFromReserve,
                                                                get_NCols(), get_NRows());
      QxrdMaskDataPtr novf = QxrdAllocator::newMask(m_Allocator,
                                                    QxrdAllocator::AllocateFromReserve,
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
      printMessage(tr("Clearing took %1 msec").arg(acqTimer.restart()));
    }

    if (i != 0) {
      for (int k=0; k<skipBetween; k++) {
        if (cancelling()) goto cancel;
        printMessage(tr("Skipping %1 of %2").arg(k+1).arg(skipBetween));
        acquireFrame(exposure);

        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          printMessage(tr("Frame after %1 msec").arg(acqTimer.restart()));
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
          printMessage(tr("accumulateAcquiredImage %1 msec idx:%2 post:%3 sum: %4 ph:%5")
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
            printMessage(tr("processAcquiredImage(line %1) %2 msec idx:%3 pre:%4 ph:%5")
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
          printMessage(tr("processAcquiredImage(line %1) %2 msec idx:%3 pre:%4 ph:%5")
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

  statusMessage(tr("Acquisition complete"));
  printMessage(tr("Acquisition complete"));

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

  printMessage(tr("Starting dark acquisition : QxrdAcquisition::doAcquireDark(fileBase = \"%1\" : index = %2 : Expos = %3 : nsum = %4 skip = %5)")
               .arg(fileBase).arg(fileIndex).arg(exposure).arg(nsummed).arg(skipBefore));

  if (synchronizedAcquisition()) {
    synchronizedAcquisition()->prepareForDarkAcquisition(&parms);
  }

  QxrdInt32ImageDataPtr res = QxrdAllocator::newInt32Image(m_Allocator,
                                                           QxrdAllocator::AllocateFromReserve,
                                                           get_NCols(), get_NRows());
  QxrdMaskDataPtr overflow  = QxrdAllocator::newMask(m_Allocator,
                                                     QxrdAllocator::AllocateFromReserve,
                                                     get_NCols(), get_NRows(),0);
  QString fb, fn;

  if (res == NULL || overflow == NULL) {
    criticalMessage("Insufficient memory for acquisition operation");
    goto cancel;
  }

  getFileBaseAndName(fileBase, fileIndex, -1, 1, fb, fn);

  res -> set_FileBase(fb);
  res -> set_FileName(fn);

  for (int i=0; i<skipBefore; i++) {
    if (cancelling()) goto cancel;
    printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
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

//saveCancel:
  processDarkImage(fileBase, fileIndex, res, overflow);

  statusMessage(tr("Acquisition complete"));
  printMessage(tr("Acquisition complete"));

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

  if (get_AcquisitionCancelsLiveView()) {
    set_LiveViewAtIdle(false);
  }

  beginAcquisition();
}

void QxrdAcquisition::startIdling()
{
  m_Idling.fetchAndStoreOrdered(1);
  endAcquisition();
}

void QxrdAcquisition::onIdleTimeout()
{
  if (m_Idling.fetchAndAddOrdered(0)) {
    QxrdInt16ImageDataPtr res = acquireFrameIfAvailable(get_ExposureTime());
    QxrdDataProcessorPtr proc(m_DataProcessor);

    if (res && proc) {
      res -> set_ExposureTime(get_ExposureTime());
      res -> set_DateTime(QDateTime::currentDateTime());
      res -> set_HBinning(1);
      res -> set_VBinning(1);
      res -> set_CameraGain(get_CameraGain());
      res -> set_DataType(QxrdInt32ImageData::Raw32Data);
      res -> set_UserComment1(get_UserComment1());
      res -> set_UserComment2(get_UserComment2());
      res -> set_UserComment3(get_UserComment3());
      res -> set_UserComment4(get_UserComment4());
      res -> set_ImageSaved(false);

      proc->idleInt16Image(res, this->get_LiveViewAtIdle());
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
    QxrdAllocatorPtr alloc(m_Allocator);

    if (alloc) {
      printMessage(tr("acquireFrame(%1) : allocated %2 MB : %3 images available")
                   .arg(exposure)
                   .arg(alloc->allocatedMemoryMB())
                   .arg(m_NAcquiredImages.available())
                   );
    }
  }

  m_NAcquiredImages.acquire(1);

  QxrdInt16ImageDataPtr res = m_AcquiredImages.dequeue();

  if (qcepDebug(DEBUG_EXTRAINPUTS) && res) {
    QcepDoubleList extra = res->get_ExtraInputs();

    for (int i=0; i<extra.count(); i++) {
      printMessage(tr("QxrdAcquisition::acquireFrame : Extra Inputs[%1] = %2").arg(i).arg(extra[i]));
    }
  }

  return res;
}

QxrdInt16ImageDataPtr QxrdAcquisition::acquireFrameIfAvailable(double exposure)
{
  if (qcepDebug(DEBUG_ACQUIRE)) {
    QxrdAllocatorPtr alloc(m_Allocator);

    if (alloc) {
      printMessage(tr("acquireFrameIfAvailable(%1) : allocated %2 MB").arg(exposure).arg(alloc->allocatedMemoryMB()));
    }
  }

  QxrdInt16ImageDataPtr res;

  while (m_NAcquiredImages.available() >= 1) {
    res = acquireFrame(exposure);
  }

  return res;
}

void QxrdAcquisition::enqueueAcquiredFrame(QxrdInt16ImageDataPtr img)
{
  if (m_AcquisitionExtraInputs) {
    m_AcquisitionExtraInputs->acquire();
    m_AcquisitionExtraInputs->logToImage(img);
  }

  if (qcepDebug(DEBUG_EXTRAINPUTS) && img) {
    QcepDoubleList extra = img->get_ExtraInputs();

    printMessage(tr("QxrdAcquisition::enqueueAcquiredFrame : %1 Extra Inputs").arg(extra.count()));

    for (int i=0; i<extra.count(); i++) {
      printMessage(tr("QxrdAcquisition::enqueueAcquiredFrame : Extra Inputs[%1] = %2").arg(i).arg(extra[i]));
    }
  }

  if (img) {
    img->set_QtVersion(QT_VERSION_STR);
    img->set_QxrdVersion(STR(QXRD_VERSION));
  }

  m_AcquiredImages.enqueue(img);

  m_NAcquiredImages.release(1);
}

void QxrdAcquisition::onExposureTimeChanged()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->onExposureTimeChanged();
  }
}

void QxrdAcquisition::onBinningModeChanged()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->onBinningModeChanged();
  }
}

void QxrdAcquisition::onCameraGainChanged()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->onCameraGainChanged();
  }
}


void QxrdAcquisition::setupExposureMenu(QDoubleSpinBox *cb)
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->setupExposureMenu(cb, get_ExposureTime());
  }
}

void QxrdAcquisition::setupCameraGainMenu(QComboBox *cb)
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->setupCameraGainMenu(cb, get_CameraGain());
  }
}

void QxrdAcquisition::setupCameraBinningModeMenu(QComboBox *cb)
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->setupCameraBinningModeMenu(cb, get_BinningMode());
  }
}


void QxrdAcquisition::beginAcquisition()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->beginAcquisition();
  }
}

void QxrdAcquisition::endAcquisition()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->endAcquisition();
  }
}

void QxrdAcquisition::shutdownAcquisition()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->shutdownAcquisition();
  }
}

void QxrdAcquisition::Message(QString msg)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->statusMessage(msg);
  }
}

void QxrdAcquisition::propertyList()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  const QMetaObject *meta = metaObject();

  int count = meta->propertyCount();
  int offset = 1; /*meta->propertyOffset();*/

  for (int i=offset; i<count; i++) {
    QMetaProperty metaproperty = meta->property(i);

    const char *name = metaproperty.name();
    QVariant value = property(name);

    if (g_Application) {
      g_Application->printMessage(tr("Property %1: %2 = %3").arg(i).arg(name).arg(value.toString()));
    }
  }

  QList<QByteArray> dynProps = dynamicPropertyNames();

  foreach(QByteArray name, dynProps) {
    g_Application->printMessage(tr("Dynamic Property %1 = %2").arg(QString(name)).arg(property(name).toString()));
  }
}

