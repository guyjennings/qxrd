#include "qxrddebug.h"
#include "qxrdacquisition.h"
#include "qcepmutexlocker.h"
#include "qxrddataprocessor.h"
#include "qcepallocator.h"
//#include "qxrdacquisitiondialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdwindow.h"
#include "qxrdapplication.h"
#include "qxrdacquisition-ptr.h"
//#include <QThreadPool>
#include <QtConcurrentRun>
#include <QDir>
#include <QMetaProperty>
#include "qxrdprocessargs.h"
#include "qxrddetectorproxy.h"
#include "qxrddetectorsettings.h"
//#include "qxrddetectorthread.h"
#include "qxrddetectorprocessor.h"
#include "qxrdacquisitionparameterpack.h"
#include "qxrddarkacquisitionparameterpack.h"
#include "qxrdacquisitionscalermodel.h"
#include "qxrdapplicationsettings.h"
#include "qxrdacquisitionexecutionthread.h"

QxrdAcquisition::QxrdAcquisition(QString name)
  : QcepObject(name),
    m_ExecutionThread(),
    m_Cancelling(this, "cancelling", 0, "Cancel Acquisition?"),
    m_Triggered(this, "triggered", 0, "Trigger Acquisition"),
    m_ExposureTime(this,"exposureTime",0.1, "Exposure Time (in sec)"),
    m_SkippedExposuresAtStart(this,"skippedExposuresAtStart",0, "Exposures to Skip at Start"),
    m_PhasesInGroup(this,"phasesInGroup",1, "Number of Image Phases"),
    m_CurrentPhase(this, "currentPhase", 0, "Current Acquisition Phase"),
    m_SummedExposures(this,"summedExposures",1, "Summed Exposures per Image"),
    m_DarkSummedExposures(this,"darkSummedExposures",1, "Summed Exposures in Dark Image"),
    m_CurrentSummation(this, "currentSumation", 0, "Current Acquisition Summation"),
    m_SkippedExposures(this,"skippedExposures",0, "Skipped Exposures between Images"),
    m_PreTriggerFiles(this,"preTriggerFiles",0, "Number of pre-Trigger Images"),
    m_PostTriggerFiles(this,"postTriggerFiles",1, "Number of post-Trigger Images"),
    m_CurrentFile(this, "currentFile", 0, "File Index of Current File"),
    m_FilePattern(this,"filePattern","", "File Name Pattern"),
    m_QxrdVersion(this,"qxrdVersion",STR(QXRD_VERSION), "QXRD Version Number"),
    m_QtVersion(this,"qtVersion",qVersion(), "QT Version Number"),
    m_DetectorCount(this, "detectorCount", 0, "Number of Detectors"),
    m_LastAcquired(this, "lastAcquired", 0, "Internal Acquisition Flag"),
    m_FileIndex(this,"fileIndex",0, "File Index"),
    m_FileIndexWidth(this, "fileIndexWidth", 5, "Digits in File Index Field"),
    m_FilePhaseWidth(this, "filePhaseWidth", 3, "Digits in Phase Number Field"),
    m_FileOverflowWidth(this, "fileOverflowWidth", 5, "Digits in Overflow Index Field"),
    m_DetectorNumberWidth(this, "detectorNumberWidth", 2, "Digits in detector number field"),
    m_FileBase(this,"fileBase","", "File Base"),
    m_OverflowLevel(this, "overflowLevel", 65500, "Overflow level (per exposure)"),
    m_Raw16SaveTime(this,"raw16SaveTime", 0.1, "Time to save 16 bit images"),
    m_Raw32SaveTime(this,"raw32SaveTime", 0.2, "Time to save 32 bit images"),
    m_RawSaveTime(this,"rawSaveTime", 0.2, "Time to save raw images"),
    m_DarkSaveTime(this,"darkSaveTime", 0.2, "Time to save dark images"),
    m_UserComment1(this,"userComment1","", "User Comment 1"),
    m_UserComment2(this,"userComment2","", "User Comment 2"),
    m_UserComment3(this,"userComment3","", "User Comment 3"),
    m_UserComment4(this,"userComment4","", "User Comment 4"),
    m_Normalization(this, "normalization", QcepDoubleList(), "Normalization Values"),
    m_DroppedFrames(this,"droppedFrames",0, "Number of Dropped Frames"),
    m_LiveViewAtIdle(this, "liveViewAtIdle", false, "Live View during Idle"),
    m_AcquisitionCancelsLiveView(this, "acquisitionCancelsLiveView", true, "Acquisition operations cancel live view"),
    m_RetryDropped(this, "retryDropped", false, "Automatically retry dropped frames during acquisition"),
    m_ScalerValues(this, "scalerValues", QcepDoubleVector(), "Scaler Values"),
    m_Mutex(QMutex::Recursive),
    m_SynchronizedAcquisition(NULL),
    m_AcquisitionExtraInputs(NULL),
    m_Window(),
    m_Idling(1)
{
#ifndef QT_NO_DEBUG
  printf("Constructing acquisition\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisition::QxrdAcquisition(%p)\n", this);
  }

  connect(prop_Raw16SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes);
  connect(prop_Raw32SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes);
  connect(prop_SummedExposures(), &QcepIntProperty::valueChanged,  this, &QxrdAcquisition::updateSaveTimes);
  connect(prop_DarkSummedExposures(), &QcepIntProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes);
  connect(prop_ExposureTime(), &QcepDoubleProperty::valueChanged, this, &QxrdAcquisition::onExposureTimeChanged);

  connect(&m_IdleTimer, &QTimer::timeout, this, &QxrdAcquisition::onIdleTimeout);

  m_IdleTimer.start(1000);
}


QxrdAcquisitionPtr QxrdAcquisition::newAcquisition()
{
  QxrdAcquisitionPtr acq(new QxrdAcquisition("acquisition"));

  if (qcepDebug(DEBUG_APP)) {
    acq->printMessage("QxrdAcquisition::QxrdAcquisition");
  }

  acq->addChildPtr(QxrdSynchronizedAcquisition::newSynchronizedAcquisition());

  acq->addChildPtr(QxrdAcquisitionExtraInputs::newAcquisitionExtraInputs());

  acq->setAcquisitionScalerModel(
        QxrdAcquisitionScalerModelPtr(
          new QxrdAcquisitionScalerModel(acq)));

  return acq;
}

void QxrdAcquisition::initialize()
{
  m_ExecutionThread =
      QxrdAcquisitionExecutionThreadPtr(
        new QxrdAcquisitionExecutionThread(qSharedPointerDynamicCast<QxrdAcquisition>(sharedFromThis())));

  m_ExecutionThread->start();
}

void QxrdAcquisition::addChildPtr(QcepObjectPtr child)
{
  QcepObject::addChildPtr(child);

  if (checkPointer<QxrdSynchronizedAcquisition>(child, m_SynchronizedAcquisition)) {}
  if (checkPointer<QxrdAcquisitionExtraInputs>(child, m_AcquisitionExtraInputs)) {
    connect(m_AcquisitionExtraInputs.data(), &QxrdAcquisitionExtraInputs::channelCountChanged,
            this, &QxrdAcquisition::extraInputsChanged);
  }
}

void QxrdAcquisition::removeChildPtr(QcepObjectPtr child)
{
  printMessage("Need to implement QxrdAcquisition::removeChildPtr");

  QcepObject::removeChildPtr(child);
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

  while (m_Detectors.count()) {
    QxrdDetectorSettingsPtr d = m_Detectors.takeFirst();

    if (d) {
      d->stopDetector();
    }
  }

//  int nDet = get_DetectorCount();

//  while (nDet > 0) {
//    printf("Attempting to stop acquisition while %d detectors still present\n", nDet);
//    QcepThread::sleep(5);
//    nDet = get_DetectorCount();
//  }
}

QxrdAcquisitionWPtr QxrdAcquisition::myself()
{
  return qSharedPointerDynamicCast<QxrdAcquisition>(sharedFromThis());
}

QxrdExperimentWPtr QxrdAcquisition::experiment() const
{
  QxrdExperimentWPtr expt = qSharedPointerDynamicCast<QxrdExperiment>(parentPtr());

  return expt;
}

QxrdApplicationWPtr QxrdAcquisition::application() const
{
  QxrdApplication *app = qobject_cast<QxrdApplication*>(g_Application);

  QxrdApplicationWPtr appw =
      qSharedPointerDynamicCast<QxrdApplication>(app->sharedFromThis());

  return appw;
}

QxrdDataProcessorWPtr QxrdAcquisition::dataProcessor() const
{
  QxrdExperimentPtr expt(experiment());

  if (expt) {
    return expt->dataProcessor();
  } else {
    return QxrdDataProcessorWPtr();
  }
}

void QxrdAcquisition::setAcquisitionScalerModel(QxrdAcquisitionScalerModelPtr model)
{
  m_ScalerModel = model;
}

QxrdAcquisitionScalerModelPtr QxrdAcquisition::acquisitionScalerModel() const
{
  return m_ScalerModel;
}

void QxrdAcquisition::acquire()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "acquire", Qt::BlockingQueuedConnection));
  } else if (sanityCheckAcquire()) {
    if (m_Acquiring.tryLock()) {
      set_Cancelling(false);
      set_Triggered(false);

      statusMessage("Starting acquisition");
      emit acquireStarted();

      //      QtConcurrent::run(this, &QxrdAcquisitionInterface::doAcquire, acquisitionParameterPack());

      if (m_ExecutionThread) {
        m_ExecutionThread->doAcquire();
      }
    } else {
      statusMessage("Acquisition is already in progress");
    }
  }
}

void QxrdAcquisition::acquireOnce()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "acquireOnce", Qt::BlockingQueuedConnection));
  } else if (sanityCheckAcquire()) {
    if (m_Acquiring.tryLock()) {
      set_Cancelling(false);
      set_Triggered(false);

      statusMessage("Starting acquisition");
      emit acquireStarted();

      //      QtConcurrent::run(this, &QxrdAcquisitionInterface::doAcquire, acquisitionParameterPack());

      if (m_ExecutionThread) {
        m_ExecutionThread->doAcquireOnce();
      }
    } else {
      statusMessage("Acquisition is already in progress");
    }
  }
}

void QxrdAcquisition::acquireDark()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "acquireDark", Qt::BlockingQueuedConnection));
  } else if (sanityCheckAcquireDark()) {
    if (m_Acquiring.tryLock()) {
      set_Cancelling(false);
      set_Triggered(true);

      statusMessage("Starting dark acquisition");
      emit acquireStarted();

      //      QtConcurrent::run(this, &QxrdAcquisitionInterface::doAcquireDark, darkAcquisitionParameterPack());

      if (m_ExecutionThread) {
        m_ExecutionThread->doAcquireDark();
      }
    } else {
      statusMessage("Acquisition is already in progress");
    }
  }
}

void QxrdAcquisition::trigger()
{
  set_Triggered(true);
}

void QxrdAcquisition::cancel()
{
  set_Cancelling(true);

//  m_NAcquiredImages.release(1);
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

void QxrdAcquisition::shutdown()
{
  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdAcquisition::shutdown()");
  }

  for (int i=0; i<get_DetectorCount(); i++) {
    QxrdDetectorSettingsPtr det = detector(i);

    if (det) {
     det -> shutdownAcquisition();
    }
  }

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

void QxrdAcquisition::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings);

  if (m_SynchronizedAcquisition) {
    settings->beginGroup("sync");
    m_SynchronizedAcquisition->writeSettings(settings);
    settings->endGroup();
  }

  if (m_AcquisitionExtraInputs) {
    settings->beginGroup("extrainputs");
    m_AcquisitionExtraInputs->writeSettings(settings);
    settings->endGroup();
  }

  settings->beginWriteArray("detectors");

  for (int i=0; i<m_Detectors.count(); i++) {
    settings->setArrayIndex(i);

    QxrdDetectorSettingsPtr det = m_Detectors.value(i);

    if (det) {
      det->writeSettings(settings);
    }
  }

  settings->endArray();
}

void QxrdAcquisition::openWindows()
{
  GUI_THREAD_CHECK;

  for (int i=0; i<m_Detectors.count(); i++) {
    QxrdDetectorSettingsPtr det = m_Detectors.value(i);

    if (det) {
      det->openWindow();
    }
  }
}

void QxrdAcquisition::closeWindows()
{
  GUI_THREAD_CHECK;

  for (int i=0; i<m_Detectors.count(); i++) {
    QxrdDetectorSettingsPtr det = m_Detectors.value(i);

    if (det) {
      det->closeWindow();
    }
  }
}

void QxrdAcquisition::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings);

  if (m_SynchronizedAcquisition) {
    settings->beginGroup("sync");
    m_SynchronizedAcquisition->readSettings(settings);
    settings->endGroup();
  }

  if (m_AcquisitionExtraInputs) {
    settings->beginGroup("extrainputs");
    m_AcquisitionExtraInputs->readSettings(settings);
    settings->endGroup();
  }

  int n = settings->beginReadArray("detectors");

  m_Detectors.resize(n);

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    settings->beginGroup("properties");
    int detType = settings->value("detectorType", 0).toInt();
    settings->endGroup();

    QxrdDetectorSettingsPtr det = QxrdDetectorSettings::newDetector(
          application(),
          experiment(), myself(), detType, i);

    if (det) {
      //          det->initialize();
      det->readSettings(settings);

      m_Detectors[i]       = det;

      QxrdApplicationSettings *set = qobject_cast<QxrdApplicationSettings*>(g_ApplicationSettings);

      if (set && set->get_StartDetectors() == 0) {
        det->set_Enabled(false);
      }

      det->startOrStop(det->isEnabled());
    }
  }

//  set_DetectorCount(m_Detectors.count());

  settings->endArray();
}

void QxrdAcquisition::appendDetector(int detType)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "appendDetector", Qt::BlockingQueuedConnection,
                              Q_ARG(int, detType));
  } else {
    int nDet = get_DetectorCount();

    QxrdDetectorSettingsPtr det =
        QxrdDetectorSettings::newDetector(application(), experiment(), myself(), detType, nDet);

    if (det) {
      m_Detectors.append(det);

      //        set_DetectorCount(m_Detectors.count());
    }
  }
}

void QxrdAcquisition::appendDetectorProxy(QxrdDetectorProxyPtr proxy)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "appendDetectorProxy", Qt::BlockingQueuedConnection,
                              Q_ARG(QxrdDetectorProxyPtr, proxy));
  } else {
    if (proxy) {
      QxrdDetectorSettingsPtr       detector  = proxy->detector();

      int nDet = get_DetectorCount();

      if (detector==NULL) {
        int detType = proxy->detectorType();

        detector = QxrdDetectorSettings::newDetector(
              application(), experiment(), myself(), detType, nDet);
      }

      m_Detectors.append(detector);

      //      set_DetectorCount(m_Detectors.count());

      detector->pullPropertiesfromProxy(proxy);
    }
  }
}

void QxrdAcquisition::clearDetectors()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "clearDetectors", Qt::BlockingQueuedConnection);
  } else {
    m_Detectors.resize(0);

//    set_DetectorCount(0);
  }
}

QxrdDetectorSettingsWPtr QxrdAcquisition::detector(int n)
{
  return m_Detectors.value(n);
}

QxrdDetectorSettings *QxrdAcquisition::det(int n)
{
  return m_Detectors.value(n).data();
}

void QxrdAcquisition::setWindow(QxrdWindowWPtr win)
{
  m_Window = win;
}

bool QxrdAcquisition::sanityCheckCommon()
{
  if (get_DetectorCount() == 0) {
    criticalMessage("No Detectors are configured");
    return false;
  }

  int ndet = 0;

  for (int i=0; i<get_DetectorCount(); i++) {
    QxrdDetectorSettingsPtr det = detector(i);

    if (det && det->isEnabled()) {
      ndet++;
    }
  }

  if (ndet == 0) {
    criticalMessage("No detectors are enabled");
    return false;
  }

  if (get_FilePattern() == "") {
    criticalMessage("No save file name has been provided");
    return false;
  }

  return true;
}

bool QxrdAcquisition::sanityCheckAcquire()
{
  bool res = false;

  if (sanityCheckCommon() == false) {
  } else if (get_PreTriggerFiles() < 0) {
    criticalMessage("pretrigger files cannot be < 0");
  } else if (get_PostTriggerFiles() < 0) {
    criticalMessage("posttrigger files cannot be < 0");
  } else if (get_PreTriggerFiles() + get_PostTriggerFiles() <= 0) {
    criticalMessage("pre+post trigger files must be >= 1");
  } else if (get_SummedExposures() <= 0) {
    criticalMessage("Summed exposures must be >= 1");
  } else if (get_SkippedExposures() < 0) {
    criticalMessage("Skipped exposures must be >= 0");
  } else if (get_ExposureTime() <= 0) {
    criticalMessage("Exposure time must be > 0");
  } else {
    res = true;
  }

  return res;
}

bool QxrdAcquisition::sanityCheckAcquireDark()
{
  bool res = false;

  if (sanityCheckCommon() == false) {
  } else if (get_DarkSummedExposures() <= 0) {
    criticalMessage("Dark summed exposures must be >= 0");
  } else if (get_ExposureTime() <= 0) {
    criticalMessage("Exposure time must be > 0");
  } else {
    res = true;
  }

  return res;
}

void QxrdAcquisition::onExposureTimeChanged(double expos)
{
  foreach (QxrdDetectorSettingsPtr det, m_Detectors) {
    det->changeExposureTime(expos);
  }
}

void QxrdAcquisition::configureDetector(int i)
{
  printMessage(tr("Configure Detector %1").arg(i));

  QxrdDetectorSettingsPtr det = detector(i);

  QxrdDetectorProxyPtr proxy(new QxrdDetectorProxy(detector(i), myself()));

  if (proxy && proxy->configureDetector()) {
    det->pullPropertiesfromProxy(proxy);
  }
}

void QxrdAcquisition::openDetectorControlWindow(int i)
{
  QxrdDetectorSettingsPtr det = detector(i);

  if (det) {
    det->openControlWindow();
  }
}

void QxrdAcquisition::clearDropped()
{
  prop_DroppedFrames() -> setValue(0);
}

void QxrdAcquisition::indicateDroppedFrame(int n)
{
  QString msg = tr("Frame %1 dropped [%2/%3 MB Used]")
      .arg(n)
      .arg(QcepAllocator::allocatedMemoryMB())
      .arg(QcepAllocator::availableMemoryMB());

  statusMessage(msg);
  printMessage(msg);

  prop_DroppedFrames() -> incValue(1);
}

void QxrdAcquisition::accumulateAcquiredImage(QcepImageDataBasePtr image, QcepUInt32ImageDataPtr accum, QcepMaskDataPtr overflow)
{
  QcepUInt16ImageDataPtr img16 = qSharedPointerCast<QcepUInt16ImageData>(image);

  if (img16) {
    accumulateAcquiredImage(img16, accum, overflow);
  } else {
    QcepUInt32ImageDataPtr img32 = qSharedPointerCast<QcepUInt32ImageData>(image);

    if (img32) {
      accumulateAcquiredImage(img32, accum, overflow);
    } else {
      printMessage("type conversion failed in QxrdAcquisition::accumulateAcquiredImage");
    }
  }
}

void QxrdAcquisition::accumulateAcquiredImage(QcepUInt16ImageDataPtr image, QcepUInt32ImageDataPtr accum, QcepMaskDataPtr overflow)
{
  if (image && accum && overflow) {
    long nPixels = image->get_Height()*image->get_Width();
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

void QxrdAcquisition::accumulateAcquiredImage(QcepUInt32ImageDataPtr image, QcepUInt32ImageDataPtr accum, QcepMaskDataPtr overflow)
{
  if (image && accum && overflow) {
    long nPixels = image->get_Height()*image->get_Width();
    quint32 ovflwlvl = get_OverflowLevel();
    quint32* src = image->data();
    quint32* dst = accum->data();
    short int* ovf = overflow->data();
    int nsummed = accum->get_SummedExposures();

    if (nsummed == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 saved").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint32 val = *src++;

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
        quint32 val = *src++;

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

QString QxrdAcquisition::currentFileBase(int detNum, QString extension)
{
  QString fileBase, fileName;

  QxrdDetectorSettingsPtr det = detector(detNum);
  QString extent;

  if (extension.length()) {
    extent = extension;
  } else if (det) {
    extent = det->get_Extension();
  } else {
    extent = "tif";
  }

  getFileBaseAndName(
        get_FilePattern(),
        extent,
        detNum,
        get_CurrentFile(),
        get_CurrentPhase(),
        get_PhasesInGroup(),
        fileBase, fileName);

  return fileBase;
}

void QxrdAcquisition::getFileBaseAndName(QString filePattern, QString extent, int detNum, int fileIndex, int phase, int nphases, QString &fileBase, QString &fileName)
{
  int width = get_FileIndexWidth();
  int detWidth = get_DetectorNumberWidth();
  QxrdDetectorSettingsPtr det(detector(detNum));

  if (det) {
    QxrdDetectorProcessorPtr proc(det->processor());
    int nDet = get_DetectorCount();

    if (proc) {
      if (nphases == 0 || phase < 0) {
        if (nDet <= 1) {
          fileBase = tr("%1-%2.dark.%3")
              .arg(filePattern).arg(fileIndex,width,10,QChar('0')).arg(extent);
        } else {
          fileBase = tr("%1-%2-%3.dark.%4")
              .arg(filePattern).arg(detNum,detWidth,10,QChar('0')).arg(fileIndex,width,10,QChar('0')).arg(extent);
        }

        fileName = proc -> filePathInDarkOutputDirectory(fileBase);
      } else {
        if (nphases > 1) {
          int phswidth = get_FilePhaseWidth();
          if (nDet <= 1) {
            fileBase = tr("%1-%2-%3.%4")
                .arg(filePattern).arg(fileIndex,width,10,QChar('0')).arg(phase,phswidth,10,QChar('0')).arg(extent);
          } else {
            fileBase = tr("%1-%2-%3-%4.%5")
                .arg(filePattern).arg(detNum,detWidth,10,QChar('0')).arg(fileIndex,width,10,QChar('0'))
                .arg(phase,phswidth,10,QChar('0')).arg(extent);
          }
        } else {
          if (nDet <= 1) {
            fileBase = tr("%1-%2.%3")
                .arg(filePattern).arg(fileIndex,width,10,QChar('0')).arg(extent);
          } else {
            fileBase = tr("%1-%2-%3.%4")
                .arg(filePattern).arg(detNum,detWidth,10,QChar('0')).arg(fileIndex,width,10,QChar('0')).arg(extent);
          }
        }
        fileName = proc -> filePathInRawOutputDirectory(fileBase);
      }
    }
  }
}

void QxrdAcquisition::processImage(QString filePattern, QString extent, int fileIndex, int phase, int nPhases, bool trig, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow)
{
  if (image) {
    if (qcepDebug(DEBUG_ACQUIRE) || qcepDebug(DEBUG_ACQUIRETIME)) {
      printMessage(tr("processAcquiredImage(%1,%2) %3 summed exposures")
                   .arg(fileIndex).arg(phase).arg(image->get_SummedExposures()));
    }

    QString fileName;
    QString fileBase;

    getFileBaseAndName(filePattern, extent, 0, fileIndex, phase, nPhases, fileBase, fileName);

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
    image -> set_Name(finfo.fileName());
    image -> set_ExposureTime(get_ExposureTime());
    image -> set_DateTime(now);
    image -> set_TimeStamp(msec);
    image -> set_HBinning(1);
    image -> set_VBinning(1);
    image -> set_DataType(QcepUInt32ImageData::Raw32Data);
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

    QxrdDataProcessorPtr processor(dataProcessor());

    if (processor) {
      processor -> acquiredInt32Image(image, overflow);
    }
  }
}

void QxrdAcquisition::processImage(const QxrdProcessArgs &args)
{
//  QThread::currentThread()->setObjectName("processImage");

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    printMessage(tr("QxrdAcquisition::processImage %1 %2 start").arg(args.m_FilePattern).arg(args.m_FileIndex));
  }

  processImage(args.m_FilePattern, args.m_Extension, args.m_FileIndex, args.m_Phase, args.m_NPhases, args.m_Trig, args.m_Image, args.m_Overflow);

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    printMessage(tr("QxrdAcquisition::processImage %1 %2 end").arg(args.m_FilePattern).arg(args.m_FileIndex));
  }
}

void QxrdAcquisition::processAcquiredImage(QString filePattern, QString extent, int fileIndex, int phase, int nPhases, bool trig, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow)
{
  //  printf("processAcquiredImage(""%s"",%d,%d,img,ovf)\n", qPrintable(filePattern), fileIndex, phase);

  //  processImage(filePattern, fileIndex, phase, nPhases, trig, image, overflow);

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    printMessage(tr("QxrdAcquisition::processAcquiredImage %1 %2 %3 (%4)").arg(filePattern).arg(fileIndex).arg(phase).arg((image?image->get_ImageNumber():-1)));
  }

  QtConcurrent::run(this, &QxrdAcquisition::processImage,
                    QxrdProcessArgs(filePattern, extent, fileIndex, phase, nPhases, trig, image, overflow));
}

void QxrdAcquisition::processDarkImage(QString filePattern, QString extent, int fileIndex, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow)
{
  //  printf("processDarkImage(""%s"",%d,img,ovf)\n", qPrintable(filePattern), fileIndex);

  processImage(filePattern, extent, fileIndex, -1, 0, true, image, overflow);
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

void QxrdAcquisition::doAcquireOnce()
{
  QxrdAcquisitionParameterPackPtr parmsp = acquisitionParameterPack();

  if (parmsp) {
    parmsp->forceSingle();

    executeAcquisition(parmsp);
  }
}

void QxrdAcquisition::doAcquire()
{
  QxrdAcquisitionParameterPackPtr parmsp = acquisitionParameterPack();

  if (parmsp) {
    executeAcquisition(parmsp);
  }
}

void QxrdAcquisition::executeAcquisition(QxrdAcquisitionParameterPackPtr parmsp)
{
  if (parmsp) {
    QTime acqTimer;
    acqTimer.start();

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

    int nDet = 0;
    QVector<QxrdDetectorSettingsPtr> dets;
    QVector<QxrdDetectorProcessorPtr> procs;
    QVector<QVector<QVector<QcepUInt32ImageDataPtr> > >res;
    QVector<QVector<QVector<QcepMaskDataPtr> > >      ovf;

    for (int i=0; i<get_DetectorCount(); i++) {
      QxrdDetectorSettingsPtr det = detector(i);

      if (det && det->isEnabled()) {
        nDet++;
        dets.append(det);
        procs.append(det->processor());

        res.resize(nDet);
        ovf.resize(nDet);

        res[nDet-1].resize(nphases);
        ovf[nDet-1].resize(nphases);

        for (int p=0; p<nphases; p++) {
          res[nDet-1][p].resize(preTrigger+1);
          ovf[nDet-1][p].resize(preTrigger+1);
        }
      }
    }


    if (synchronizedAcquisition()) {
      synchronizedAcquisition()->prepareForAcquisition(parmsp);
    }

    if (acquisitionExtraInputs()) {
      acquisitionExtraInputs()->prepareForAcquisition(parmsp);
    }

    for (int d=0; d<nDet; d++) {
      dets[d] -> beginAcquisition(exposure);
    }

    printMessage(tr("acquire(\"%1\", %2, %3, %4, %5, %6) // fileIndex = %7")
                 .arg(fileBase).arg(exposure).arg(nsummed).arg(postTrigger).arg(preTrigger).arg(nphases).arg(fileIndex));


    set_CurrentPhase(0);
    set_CurrentSummation(-1);
    set_CurrentFile(fileIndex);

    for (int i=0; i<skipBefore; i++) {
      if (cancelling()) goto cancel;
      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
      }

      for (int d=0; d<nDet; d++) {
        dets[d]->beginFrame();
      }

      for (int d=0; d<nDet; d++) {
        dets[d]->acquireFrame();
      }
    }

    for (int i=0; i<postTrigger; i += (get_Triggered() ? 1:0)) {
      if (cancelling()) goto cancel;

      set_CurrentFile(fileIndex);

      if (i != 0) {
        for (int k=0; k<skipBetween; k++) {
          if (cancelling()) goto cancel;
          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Skipping %1 of %2").arg(k+1).arg(skipBetween));
          }

          for (int d=0; d<nDet; d++) {
            dets[d]->acquireFrame();
          }

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Frame after %1 msec").arg(acqTimer.restart()));
          }
        }
      }

      if (cancelling()) goto cancel;

      for (int s=0; s<nsummed;) {
        set_CurrentSummation(s);

        for (int p=0; p<nphases; p++) {
          set_CurrentPhase(p);

          for (int d=0; d<nDet; d++) {
            QxrdDetectorSettingsPtr det = dets[d];

            det->beginFrame();
          }

          for (int d=0; d<nDet; d++) {
            QxrdDetectorSettingsPtr det = dets[d];
            int nCols = det->get_NCols();
            int nRows = det->get_NRows();

            if (res[d][p][0] == NULL) {
              QcepUInt32ImageDataPtr nres = QcepAllocator::newInt32Image(tr("acc-%1-%2").arg(d).arg(p),
                                                                        nCols, nRows,
                                                                        QcepAllocator::AllocateFromReserve);

              res[d][p][0] = nres;

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

                getFileBaseAndName(fileBase, det->get_Extension(),
                                   det->get_DetectorNumber(),
                                   fileIndex, p, nphases, fb, fn);

                nres -> set_FileBase(fb);
                nres -> set_FileName(fn);
              }
            }

            if (ovf[d][p][0] == NULL) {
              QcepMaskDataPtr novf = QcepAllocator::newMask(tr("mask-%1-%2").arg(d).arg(p),
                                                            nCols, nRows, 0,
                                                            QcepAllocator::AllocateFromReserve);
              ovf[d][p][0] = novf;

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

              if (novf) ovf[d][p][0] -> set_SummedExposures(0);
            }

            if (res[d][p][0]) {
              emit acquiredFrame(res[d][p][0]->get_FileBase(),
                  p, nphases,
                  s, nsummed,
                  i, postTrigger);
            }

            QcepImageDataBasePtr img = det -> acquireFrame();

            if (img && res[d][p][0] && ovf[d][p][0]) {
              accumulateAcquiredImage(img, res[d][p][0], ovf[d][p][0]);
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
        }

        if (get_RetryDropped()) {
          int minSum = nsummed+10;

          for (int p=0; p<nphases; p++) {
            for (int d=0; d<nDet; d++) {
              if (res[d][p][0]) {
                int ns = res[d][p][0]->get_SummedExposures();

                if (ns < minSum) {
                  minSum = ns;
                }
              } else {
                minSum = 0;
              }
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
            for (int d=0; d<nDet; d++) {

              procs[d] -> processAcquiredImage(res[d][p][ii], ovf[d][p][ii], fileIndex, p, nphases, false);

              if (qcepDebug(DEBUG_ACQUIRETIME)) {
                printMessage(tr("processAcquiredImage(line %1) %2 msec idx:%3 pre:%4 ph:%5")
                             .arg(__LINE__)
                             .arg(acqTimer.restart())
                             .arg(fileIndex)
                             .arg(ii)
                             .arg(p)
                             );
              }

              res[d][p].pop_back();
              ovf[d][p].pop_back();
            }
          }

          fileIndex++;
          set_FileIndex(fileIndex);
        }

        nPreTriggered = 0;

        for (int p=0; p<nphases; p++) {
          for (int d=0; d<nDet; d++) {
            procs[d] -> processAcquiredImage(res[d][p][0], ovf[d][p][0], fileIndex, p, nphases, true);

            if (qcepDebug(DEBUG_ACQUIRETIME)) {
              printMessage(tr("processAcquiredImage(line %1) %2 msec idx:%3 pre:%4 ph:%5")
                           .arg(__LINE__)
                           .arg(acqTimer.restart())
                           .arg(fileIndex)
                           .arg(i)
                           .arg(p)
                           );
            }

            res[d][p][0] = QcepUInt32ImageDataPtr();
            ovf[d][p][0] = QcepMaskDataPtr();
          }
        }
        fileIndex++;
        set_FileIndex(fileIndex);
      } else {
        nPreTriggered++;
        for (int p=0; p<nphases; p++) {
          for (int d=0; d<nDet; d++) {
            res[d][p].push_front(QcepUInt32ImageDataPtr());
            ovf[d][p].push_front(QcepMaskDataPtr());
            res[d][p].pop_back();
            ovf[d][p].pop_back();
          }
        }
      }
    }

    statusMessage(tr("Acquisition complete"));
    printMessage(tr("Acquisition complete"));

cancel:
    if (synchronizedAcquisition()) {
      synchronizedAcquisition()->finishedAcquisition();
    }

    for (int d=0; d<nDet; d++) {
      dets[d]->endAcquisition();
    }

    QVector<double> scalers;

    scalers.append(get_FileIndex()-1);

    if (acquisitionExtraInputs()) {
      scalers += acquisitionExtraInputs()->evaluateChannels();
    }

    for (int d=0; d<nDet; d++) {
      scalers += dets[d]->scalerCounts();
    }

    set_ScalerValues(scalers);

    startIdling();
  }

  m_Acquiring.unlock();

  emit acquireComplete();
}

void QxrdAcquisition::doAcquireDark()
{
  QxrdDarkAcquisitionParameterPackPtr parmsp = darkAcquisitionParameterPack();

  if (parmsp) {
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

    int nDet = 0;
    QVector<QxrdDetectorSettingsPtr> dets;
    QVector<QxrdDetectorProcessorPtr> procs;
    QVector<QcepUInt32ImageDataPtr> res;
    QVector<QcepMaskDataPtr> overflow;

    set_CurrentPhase(-1);
    set_CurrentSummation(0);
    set_CurrentFile(fileIndex);

    for (int i=0; i<get_DetectorCount(); i++) {
      QxrdDetectorSettingsPtr det = detector(i);

      if (det && det->isEnabled()) {
        nDet++;
        dets.append(det);
        procs.append(det->processor());

        QcepUInt32ImageDataPtr img = QcepAllocator::newInt32Image(tr("dark-%1").arg(i),
                                                                 det->get_NCols(), det->get_NRows(),
                                                                 QcepAllocator::AllocateFromReserve);

        res.append(img);

        QcepMaskDataPtr msk = QcepAllocator::newMask(tr("mask-%1").arg(i),
                                                     det->get_NCols(), det->get_NRows(),0,
                                                     QcepAllocator::AllocateFromReserve);

        overflow.append(msk);
      }
    }

    if (synchronizedAcquisition()) {
      synchronizedAcquisition()->prepareForDarkAcquisition(parmsp);
    }

    QString fb, fn;

    for (int d=0; d<nDet; d++) {
      if (res[d] == NULL || overflow[d] == NULL) {
        criticalMessage("Insufficient memory for acquisition operation");
        goto cancel;
      }
    }

    for (int d=0; d<nDet; d++) {
      dets[d] -> beginAcquisition(exposure);
    }

    for (int d=0; d<nDet; d++) {
      QxrdDetectorSettingsPtr det = dets[d];

      getFileBaseAndName(fileBase, det->get_Extension(), det->get_DetectorNumber(), fileIndex, -1, 1, fb, fn);

      res[d] -> set_FileBase(fb);
      res[d] -> set_FileName(fn);
    }

    for (int i=0; i<skipBefore; i++) {
      if (cancelling()) goto cancel;

      set_CurrentSummation(-1);

      for (int d=0; d<nDet; d++) {
        dets[i] -> beginFrame();
      }

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
      }

      for (int d=0; d<nDet; d++) {
        dets[i] -> acquireFrame();
      }
    }

    for (int i=0; i<nsummed;) {
      if (cancelling()) goto cancel;

      set_CurrentSummation(i);

      for (int d=0; d<nDet; d++) {
        dets[d] -> beginFrame();
      }

      for (int d=0; d<nDet; d++) {
        emit acquiredFrame(res[d]->get_FileBase(), 0, 1, i, nsummed, 0, 1);

        QcepImageDataBasePtr img =
            dets[d] -> acquireFrame();

        if (img) {
          accumulateAcquiredImage(img, res[d], overflow[d]);
        } else {
          if (!cancelling()){
            indicateDroppedFrame(0);
          } else {
            goto cancel;
          }
        }
      }

      if (get_RetryDropped()) {
        int minSum = nsummed+10;

        for (int d=0; d<nDet; d++) {
          if (res[d]) {
            int ns = res[d]->get_SummedExposures();

            if (ns < minSum) {
              minSum = ns;
            }
          } else {
            minSum = 0;
          }
        }

        i = minSum;
      } else{
        i = i+1;
      }
    }

    //saveCancel:

    for (int d=0; d<nDet; d++) {
      procs[d]->processDarkImage(res[d], overflow[d], fileIndex);
    }

    statusMessage(tr("Acquisition complete"));
    printMessage(tr("Acquisition complete"));

cancel:
    if (synchronizedAcquisition()) {
      synchronizedAcquisition()->finishedAcquisition();
    }

    startIdling();
  }

  m_Acquiring.unlock();

  emit acquireComplete();
}

void QxrdAcquisition::stopIdling()
{
  m_Idling.fetchAndStoreOrdered(0);
//  flushImageQueue();

  if (get_AcquisitionCancelsLiveView()) {
    set_LiveViewAtIdle(false);
  }
}

void QxrdAcquisition::startIdling()
{
  m_Idling.fetchAndStoreOrdered(1);
}

void QxrdAcquisition::onIdleTimeout()
{
  if (m_Idling.fetchAndAddOrdered(0)) {
    QTime tic;
    tic.start();

    for (int i=0; i<get_DetectorCount(); i++) {
      QxrdDetectorSettingsPtr det = detector(i);

      if (det && det->isEnabled()) {
        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          printMessage(tr("Det %1 is enabled after %2 msec").arg(i).arg(tic.restart()));
        }

        QcepImageDataBasePtr res = det -> acquireFrameIfAvailable();
        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          printMessage(tr("Tried to acquire frame from det %1 after %2 msec").arg(i).arg(tic.restart()));
        }

        QxrdDetectorProcessorPtr proc = det->processor();

        if (proc) {
          proc->processIdleImage(res);

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Processed idle frame from det %1 after %2 msec").arg(i).arg(tic.restart()));
          }
        }
      }
    }
  }
}

void QxrdAcquisition::Message(QString msg)
{
  QxrdExperimentPtr expt(experiment());

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

//TODO: still needed?
void QxrdAcquisition::setupExposureMenu(QDoubleSpinBox * /*cb*/)
{
}

double QxrdAcquisition::scalerValue(int i)
{
  return get_ScalerValues().value(i);
}
