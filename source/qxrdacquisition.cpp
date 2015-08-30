#include "qxrddebug.h"
#include "qxrdacquisition.h"
#include "qcepmutexlocker.h"
#include "qxrddataprocessor.h"
#include "qcepallocator.h"
#include "qxrdacquisitiondialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdwindow.h"
#include "qxrdapplication.h"
#include "qxrdacquisition-ptr.h"
#include <QThreadPool>
#include <QtConcurrentRun>
#include <QDir>
#include <QMetaProperty>

QxrdAcquisition::QxrdAcquisition(QcepSettingsSaverWPtr saver,
                                 QxrdExperimentWPtr doc,
                                 QxrdDataProcessorWPtr proc,
                                 QcepAllocatorWPtr allocator)
  : QcepObject("acquisition", NULL),
    m_Saver(saver),
    m_QxrdVersion(QcepSettingsSaverWPtr(), this,"qxrdVersion",STR(QXRD_VERSION), "QXRD Version Number"),
    m_QtVersion(QcepSettingsSaverWPtr(), this,"qtVersion",qVersion(), "QT Version Number"),
//    m_DetectorType(QcepSettingsSaverWPtr(), this, "detectorType", 0, "Detector Type (0 = simulated, 1 = PE, 2 = Pilatus, 3 = EPICS, 4 = Files)"),
    m_ExposureTime(saver, this,"exposureTime",0.1, "Exposure Time (in sec)"),
    m_SkippedExposuresAtStart(saver, this,"skippedExposuresAtStart",0, "Exposures to Skip at Start"),
    m_LastAcquired(QcepSettingsSaverWPtr(), this, "lastAcquired", 0, "Internal Acquisition Flag"),
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
    m_AcquireDark(QcepSettingsSaverWPtr(), this, "acquireDark", 0, "Acquire Dark Image?"),
    m_Cancelling(QcepSettingsSaverWPtr(), this, "cancelling", 0, "Cancel Acquisition?"),
    m_Triggered(QcepSettingsSaverWPtr(), this, "triggered", 0, "Trigger Acquisition"),
    m_Raw16SaveTime(saver, this,"raw16SaveTime", 0.1, "Time to save 16 bit images"),
    m_Raw32SaveTime(saver, this,"raw32SaveTime", 0.2, "Time to save 32 bit images"),
    m_RawSaveTime(saver, this,"rawSaveTime", 0.2, "Time to save raw images"),
    m_DarkSaveTime(saver, this,"darkSaveTime", 0.2, "Time to save dark images"),
    m_UserComment1(saver, this,"userComment1","", "User Comment 1"),
    m_UserComment2(saver, this,"userComment2","", "User Comment 2"),
    m_UserComment3(saver, this,"userComment3","", "User Comment 3"),
    m_UserComment4(saver, this,"userComment4","", "User Comment 4"),
    m_Normalization(saver, this, "normalization", QcepDoubleList(), "Normalization Values"),
    m_DroppedFrames(QcepSettingsSaverWPtr(), this,"droppedFrames",0, "Number of Dropped Frames"),
    m_LiveViewAtIdle(saver, this, "liveViewAtIdle", false, "Live View during Idle"),
    m_AcquisitionCancelsLiveView(saver, this, "acquisitionCancelsLiveView", true, "Acquisition operations cancel live view"),
    m_RetryDropped(saver, this, "retryDropped", false, "Automatically retry dropped frames during acquisition"),
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
  connect(prop_Raw16SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes);
  connect(prop_Raw32SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes);
  connect(prop_SummedExposures(), &QcepIntProperty::valueChanged,  this, &QxrdAcquisition::updateSaveTimes);
  connect(prop_DarkSummedExposures(), &QcepIntProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes);

//  m_FileIndex.setDebug(1);

  QcepAllocatorPtr alloc(m_Allocator);

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdAcquisition::QxrdAcquisition");
  }

  m_SynchronizedAcquisition = QxrdSynchronizedAcquisitionPtr(
        new QxrdSynchronizedAcquisition(m_Saver, sharedFromThis()));

  m_AcquisitionExtraInputs = QxrdAcquisitionExtraInputsPtr(new QxrdAcquisitionExtraInputs(m_Saver, m_Experiment, sharedFromThis()));
  m_AcquisitionExtraInputs -> initialize(m_AcquisitionExtraInputs);

  connect(prop_ExposureTime(), &QcepDoubleProperty::valueChanged,
          this, &QxrdAcquisition::onExposureTimeChanged);

  connect(prop_BinningMode(), &QcepIntProperty::valueChanged,
          this, &QxrdAcquisition::onBinningModeChanged);

  connect(prop_CameraGain(), &QcepIntProperty::valueChanged,
          this, &QxrdAcquisition::onCameraGainChanged);

  if (alloc) {
    if (sizeof(void*) == 4) {
      connect(alloc->prop_TotalBufferSizeMB32(), &QcepIntProperty::valueChanged,
              this, &QxrdAcquisition::onBufferSizeChanged);
      onBufferSizeChanged(alloc->get_TotalBufferSizeMB32());
    } else {
      connect(alloc->prop_TotalBufferSizeMB64(), &QcepIntProperty::valueChanged,
              this, &QxrdAcquisition::onBufferSizeChanged);
      onBufferSizeChanged(alloc->get_TotalBufferSizeMB64());
    }
  }

  connect(&m_Watcher, &QFutureWatcherBase::finished, this, &QxrdAcquisition::onAcquireComplete);
  connect(&m_IdleTimer, &QTimer::timeout, this, &QxrdAcquisition::onIdleTimeout);

  m_IdleTimer.start(1000);
}

QxrdAcquisition::~QxrdAcquisition()
{
#ifndef QT_NO_DEBUG
  printf("Deleting acquisition\n");
#endif

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition->writeSettings(settings, section+"/sync");
  }

  if (m_AcquisitionExtraInputs) {
    m_AcquisitionExtraInputs->writeSettings(settings, section+"/extrainputs");
  }

  QcepObject::writeSettings(settings, section);
}

void QxrdAcquisition::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition->readSettings(settings, section+"/sync");
  }

  if (m_AcquisitionExtraInputs) {
    m_AcquisitionExtraInputs->readSettings(settings, section+"/extrainputs");
  }

  QcepObject::readSettings(settings, section);
}

void QxrdAcquisition::setWindow(QxrdWindowWPtr win)
{
  m_Window = win;
}

void QxrdAcquisition::setDetector(QxrdDetectorWPtr det)
{
  m_Detector = det;
}

void QxrdAcquisition::printMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdAcquisition::criticalMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->criticalMessage(msg, ts);
  }
}

void QxrdAcquisition::statusMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->statusMessage(msg, ts);
  }
}

void QxrdAcquisition::onBufferSizeChanged(int newMB)
{
  QcepAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    alloc -> changedSizeMB(newMB);
  }
}

QcepAllocatorWPtr QxrdAcquisition::allocator() const
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

      QFuture<void> res = QtConcurrent::run(this, &QxrdAcquisition::doAcquire, acquisitionParameterPack());

      m_Watcher.setFuture(res);
    } else {
      statusMessage("Acquisition is already in progress");
    }
  }
}

QxrdAcquisitionParameterPackPtr QxrdAcquisition::acquisitionParameterPack()
{
  return QxrdAcquisitionParameterPackPtr(
        new QxrdAcquisitionParameterPack (get_FilePattern(),
                                          get_ExposureTime(),
                                          get_SummedExposures(),
                                          get_PreTriggerFiles(),
                                          get_PostTriggerFiles(),
                                          get_PhasesInGroup(),
                                          get_SkippedExposuresAtStart(),
                                          get_SkippedExposures()));
}

QxrdDarkAcquisitionParameterPackPtr QxrdAcquisition::darkAcquisitionParameterPack()
{
  return QxrdDarkAcquisitionParameterPackPtr(
        new QxrdDarkAcquisitionParameterPack(get_FilePattern(),
                                          get_ExposureTime(),
                                          get_DarkSummedExposures(),
                                          get_SkippedExposuresAtStart()));

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

      QFuture<void> res = QtConcurrent::run(this, &QxrdAcquisition::doAcquireDark, darkAcquisitionParameterPack());

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
  QcepMutexLocker lock(__FILE__, __LINE__, &mutex);

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
  QcepAllocatorPtr alloc(m_Allocator);

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

void QxrdAcquisition::accumulateAcquiredImage(QcepInt16ImageDataPtr image, QcepInt32ImageDataPtr accum, QcepMaskDataPtr overflow)
{
  if (image && accum && overflow) {
    long nPixels = get_NRows()*get_NCols();
    int ovflwlvl = get_OverflowLevel();
    quint16* src = image->data();
    quint32* dst = accum->data();
    short int* ovf = overflow->data();
    int nsummed = accum->get_SummedExposures();

    if (nsummed == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 saved").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint16 val = *src++;

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
      accum->set_ImageSequenceNumber(image->get_ImageSequenceNumber());
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 summed").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint16 val = *src++;

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
      accum->prop_ImageSequenceNumber() -> incValue(image->get_ImageSequenceNumber());
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

void QxrdAcquisition::processImage(QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow)
{
  if (image) {
//    int w=image->get_Width(), h=image->get_Height();

//    QcepInt32ImageDataPtr proc = QcepAllocator::newInt32Image(m_Allocator, QcepAllocator::AllocateFromReserve, w,h);
//    QcepMaskDataPtr ovf = QcepAllocator::newMask(m_Allocator, QcepAllocator::AllocateFromReserve, w,h, 0);

//    if (proc == NULL || ovf == NULL) {
//      indicateDroppedFrame(0);
//      return;
//    }

//    proc->copyFrom(image);
//    overflow->copyMaskTo(ovf);

    if (qcepDebug(DEBUG_ACQUIRE) || qcepDebug(DEBUG_ACQUIRETIME)) {
      printMessage(tr("processAcquiredImage(%1,%2) %3 summed exposures")
                   .arg(fileIndex).arg(phase).arg(image->get_SummedExposures()));
    }

    QString fileName;
    QString fileBase;

    getFileBaseAndName(filePattern, fileIndex, phase, nPhases, fileBase, fileName);

    if (qcepDebug(DEBUG_ACQUIRE) || qcepDebug(DEBUG_ACQUIRETIME)) {
      printMessage(tr("Fn: %1, Fi: %2, Phs: %3")
                   .arg(fileName).arg(fileIndex).arg(phase));
    }

    set_FileBase(fileBase);
    //        m_DataProcessor -> set_FileName(fileName);

    QFileInfo finfo(fileName);

    QDateTime now = QDateTime::currentDateTime();
    double msec = QcepImageDataBase::secondsSinceEpoch();

    image -> set_FileBase(fileBase);
    image -> set_FileName(fileName);
    image -> set_Title(finfo.fileName());
    image -> set_ExposureTime(get_ExposureTime());
    image -> set_DateTime(now);
    image -> set_TimeStamp(msec);
    image -> set_HBinning(1);
    image -> set_VBinning(1);
    image -> set_CameraGain(get_CameraGain());
    image -> set_DataType(QcepInt32ImageData::Raw32Data);
    image -> set_UserComment1(get_UserComment1());
    image -> set_UserComment2(get_UserComment2());
    image -> set_UserComment3(get_UserComment3());
    image -> set_UserComment4(get_UserComment4());
    image -> set_ObjectSaved(false);
    image -> set_Triggered(trig);
    image -> set_Normalization(get_Normalization());

    copyDynamicProperties(image.data());

    if (nPhases == 0) {
      if (qcepDebug(DEBUG_ACQUIRE) || qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("32 bit Dark Image acquired"));
      }

      image -> set_ImageNumber(-1);
      image -> set_PhaseNumber(-1);
      image -> set_NPhases(0);
    } else {
      if (qcepDebug(DEBUG_ACQUIRE) || qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("32 bit Image %1 acquired").arg(fileIndex));
      }

      image -> set_ImageNumber(fileIndex);
      image -> set_PhaseNumber(phase);
      image -> set_NPhases(nPhases);
    }

    QxrdDataProcessorPtr processor(m_DataProcessor);

    if (processor) {
      processor -> acquiredInt32Image(image, overflow);
    }
  }
}

void QxrdAcquisition::processImage(const QxrdProcessArgs &args)
{
  QThread::currentThread()->setObjectName("processImage");

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    printMessage(tr("QxrdAcquisition::processImage %1 %2 start").arg(args.m_FilePattern).arg(args.m_FileIndex));
  }

  processImage(args.m_FilePattern, args.m_FileIndex, args.m_Phase, args.m_NPhases, args.m_Trig, args.m_Image, args.m_Overflow);

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    printMessage(tr("QxrdAcquisition::processImage %1 %2 end").arg(args.m_FilePattern).arg(args.m_FileIndex));
  }
}

void QxrdAcquisition::processAcquiredImage(QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow)
{
  //  printf("processAcquiredImage(""%s"",%d,%d,img,ovf)\n", qPrintable(filePattern), fileIndex, phase);

  //  processImage(filePattern, fileIndex, phase, nPhases, trig, image, overflow);

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    printMessage(tr("QxrdAcquisition::processAcquiredImage %1 %2 %3 (%4)").arg(filePattern).arg(fileIndex).arg(phase).arg((image?image->get_ImageNumber():-1)));
  }

  QtConcurrent::run(this, &QxrdAcquisition::processImage,
                    QxrdProcessArgs(filePattern, fileIndex, phase, nPhases, trig, image, overflow));
}

void QxrdAcquisition::processDarkImage(QString filePattern, int fileIndex, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow)
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
                                               sharedFromThis(),
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

void QxrdAcquisition::doAcquire(QxrdAcquisitionParameterPackWPtr parms)
{
  QxrdAcquisitionParameterPackPtr parmsp (parms);

  if (parmsp) {
    QTime acqTimer;
    acqTimer.start();

    QThread::currentThread()->setObjectName("doAcquire");

    stopIdling();

    QString fileBase = parmsp->fileBase();
    int fileIndex = get_FileIndex();
    double exposure = parmsp->exposure();
    int nsummed = parmsp->nsummed();
    int preTrigger = parmsp->preTrigger();
    int postTrigger = parmsp->postTrigger();
    int nphases = parmsp->nphases();
    int skipBefore = parmsp->skipBefore();

    if (skipBefore <= 0) {
      if (get_LastAcquired() != 1) {
        skipBefore = 1;
      }
    }

    set_LastAcquired(1);

    int skipBetween = parmsp->skipBetween();
    int nPreTriggered = 0;

    if (preTrigger <= 0) {
      set_Triggered(true);
    }

    if (synchronizedAcquisition()) {
      synchronizedAcquisition()->prepareForAcquisition(parms);
    }

    if (acquisitionExtraInputs()) {
      acquisitionExtraInputs()->prepareForAcquisition(parms);
    }

    QVector<QVector<QcepInt32ImageDataPtr> >res(nphases);
    QVector<QVector<QcepMaskDataPtr> >      ovf(nphases);

    printMessage(tr("acquire(\"%1\", %2, %3, %4, %5, %6) // fileIndex = %7")
                 .arg(fileBase).arg(exposure).arg(nsummed).arg(postTrigger).arg(preTrigger).arg(nphases).arg(fileIndex));

    for (int p=0; p<nphases; p++) {
      res[p].resize(preTrigger+1);
      ovf[p].resize(preTrigger+1);
    }

    for (int i=0; i<skipBefore; i++) {
      if (cancelling()) goto cancel;
      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
      }
      acquireFrame(exposure);
    }

    for (int i=0; i<postTrigger; i += (get_Triggered() ? 1:0)) {
      if (cancelling()) goto cancel;

      if (i != 0) {
        for (int k=0; k<skipBetween; k++) {
          if (cancelling()) goto cancel;
          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Skipping %1 of %2").arg(k+1).arg(skipBetween));
          }
          acquireFrame(exposure);

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Frame after %1 msec").arg(acqTimer.restart()));
          }
        }
      }

      if (cancelling()) goto cancel;

      for (int s=0; s<nsummed;) {
        for (int p=0; p<nphases; p++) {
          if (res[p][0] == NULL) {

            QcepInt32ImageDataPtr nres = QcepAllocator::newInt32Image(QcepAllocator::AllocateFromReserve,
                                                                      get_NCols(), get_NRows());
            res[p][0] = nres;

            if (nres == NULL) {
              if (qcepDebug(DEBUG_ACQUIRETIME)) {
                printMessage("Dropped frame allocation...");
              }
              indicateDroppedFrame(i);
            } else {
              QString fb, fn;
              if (qcepDebug(DEBUG_ACQUIRETIME)) {
                printMessage(tr("Newly allocated image number %1").arg(nres->get_ImageNumber()));
              }

              nres -> set_SummedExposures(0);

              getFileBaseAndName(fileBase, fileIndex+i, p, nphases, fb, fn);

              nres -> set_FileBase(fb);
              nres -> set_FileName(fn);
            }
          }

          if (ovf[p][0] == NULL) {
            QcepMaskDataPtr novf = QcepAllocator::newMask(QcepAllocator::AllocateFromReserve,
                                                          get_NCols(), get_NRows(), 0);
            ovf[p][0] = novf;

            if (novf == NULL) {
              if (qcepDebug(DEBUG_ACQUIRETIME)) {
                printMessage("Dropped mask frame allocation...");
              }
              indicateDroppedFrame(i);
            } else {
              if (qcepDebug(DEBUG_ACQUIRETIME)) {
                printMessage(tr("Newly allocated mask number %1").arg(novf->get_ImageNumber()));
              }
            }

            if (novf) ovf[p][0] -> set_SummedExposures(0);
          }

          if (res[p][0]) {
            emit acquiredFrame(res[p][0]->get_FileBase(), p, nphases, s, nsummed, i, postTrigger);
          }

          QcepInt16ImageDataPtr img = acquireFrame(exposure);

          if (img && res[p][0] && ovf[p][0]) {
            accumulateAcquiredImage(img, res[p][0], ovf[p][0]);
          } else if (!cancelling()){
            indicateDroppedFrame(i);
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

        if (get_RetryDropped()) {
          int minSum = nsummed+10;

          for (int p=0; p<nphases; p++) {
            if (res[p][0]) {
              int ns = res[p][0]->get_SummedExposures();

              if (ns < minSum) {
                minSum = ns;
              }
            } else {
              minSum = 0;
            }
          }

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("i = %1, Minsum = %2, s = %3, nsummed = %4").arg(i).arg(minSum).arg(s).arg(nsummed));
          }

          if (minSum == nsummed+10) {
            if (qcepDebug(DEBUG_ACQUIRETIME)) {
              printMessage("No acquired images allocated");
            }
            //          s = s+1;
          } else {
            s = minSum;
          }
        } else {
          s = s+1;
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
          processAcquiredImage(fileBase, fileIndex, p, nphases, true, res[p][0], ovf[p][0]);

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("processAcquiredImage(line %1) %2 msec idx:%3 pre:%4 ph:%5")
                         .arg(__LINE__)
                         .arg(acqTimer.restart())
                         .arg(fileIndex)
                         .arg(i)
                         .arg(p)
                         );
          }

          res[p][0] = QcepInt32ImageDataPtr();
          ovf[p][0] = QcepMaskDataPtr();
        }
        fileIndex++;
        set_FileIndex(fileIndex);
      } else {
        nPreTriggered++;
        for (int p=0; p<nphases; p++) {
          res[p].push_front(QcepInt32ImageDataPtr());
          ovf[p].push_front(QcepMaskDataPtr());
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
}

void QxrdAcquisition::doAcquireDark(QxrdDarkAcquisitionParameterPackWPtr parms)
{
  QxrdDarkAcquisitionParameterPackPtr parmsp(parms);

  if (parmsp) {
    QThread::currentThread()->setObjectName("doAcquireDark");

    stopIdling();

    QString fileBase = parmsp->fileBase();
    int fileIndex = get_FileIndex();
    double exposure = parmsp->exposure();
    int nsummed = parmsp->nsummed();
    int skipBefore = parmsp->skipBefore();

    if (skipBefore <= 0) {
      if (get_LastAcquired() != -1) {
        skipBefore = 1;
      }
    }

    set_LastAcquired(-1);

    printMessage(tr("acquireDark(\"%1\", %2, %3) // fileIndex = %4")
                 .arg(fileBase).arg(exposure).arg(nsummed).arg(fileIndex));

    if (synchronizedAcquisition()) {
      synchronizedAcquisition()->prepareForDarkAcquisition(parms);
    }

    QcepInt32ImageDataPtr res = QcepAllocator::newInt32Image(QcepAllocator::AllocateFromReserve,
                                                             get_NCols(), get_NRows());
    QcepMaskDataPtr overflow  = QcepAllocator::newMask(QcepAllocator::AllocateFromReserve,
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

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
      }

      acquireFrame(exposure);
    }

    for (int i=0; i<nsummed; i++) {
      if (cancelling()) goto cancel;

      emit acquiredFrame(res->get_FileBase(), 0, 1, i, nsummed, 0, 1);

      QcepInt16ImageDataPtr img = acquireFrame(exposure);

      if (img) {
        accumulateAcquiredImage(img, res, overflow);
      } else {
        if (!cancelling()){
          indicateDroppedFrame(0);
        } else {
          goto cancel;
        }
      }

      if (get_RetryDropped()) {
        i = res->get_SummedExposures() + 1;
      } else{
        i = i+1;
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
    QcepInt16ImageDataPtr res = acquireFrameIfAvailable(get_ExposureTime());
    QxrdDataProcessorPtr proc(m_DataProcessor);

    if (res && proc) {
      res -> set_ExposureTime(get_ExposureTime());
      res -> set_DateTime(QDateTime::currentDateTime());
      res -> set_HBinning(1);
      res -> set_VBinning(1);
      res -> set_CameraGain(get_CameraGain());
      res -> set_DataType(QcepInt32ImageData::Raw32Data);
      res -> set_UserComment1(get_UserComment1());
      res -> set_UserComment2(get_UserComment2());
      res -> set_UserComment3(get_UserComment3());
      res -> set_UserComment4(get_UserComment4());
      res -> set_ObjectSaved(false);

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

QcepInt16ImageDataPtr QxrdAcquisition::acquireFrame(double exposure)
{
  if (qcepDebug(DEBUG_ACQUIRE)) {
    QcepAllocatorPtr alloc(m_Allocator);

    if (alloc) {
      printMessage(tr("acquireFrame(%1) : allocated %2 MB : %3 images available")
                   .arg(exposure)
                   .arg(alloc->allocatedMemoryMB())
                   .arg(m_NAcquiredImages.available())
                   );
    }
  }

  m_NAcquiredImages.acquire(1);

  QcepInt16ImageDataPtr res = m_AcquiredImages.dequeue();

  if (qcepDebug(DEBUG_EXTRAINPUTS) && res) {
    QcepDoubleList extra = res->get_ExtraInputs();

    for (int i=0; i<extra.count(); i++) {
      printMessage(tr("QxrdAcquisition::acquireFrame : Extra Inputs[%1] = %2").arg(i).arg(extra[i]));
    }
  }

  return res;
}

QcepInt16ImageDataPtr QxrdAcquisition::acquireFrameIfAvailable(double exposure)
{
  if (qcepDebug(DEBUG_ACQUIRE)) {
    QcepAllocatorPtr alloc(m_Allocator);

    if (alloc) {
      printMessage(tr("acquireFrameIfAvailable(%1) : allocated %2 MB").arg(exposure).arg(alloc->allocatedMemoryMB()));
    }
  }

  QcepInt16ImageDataPtr res;

  while (m_NAcquiredImages.available() >= 1) {
    res = acquireFrame(exposure);
  }

  return res;
}

void QxrdAcquisition::enqueueAcquiredFrame(QcepInt16ImageDataPtr img)
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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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

