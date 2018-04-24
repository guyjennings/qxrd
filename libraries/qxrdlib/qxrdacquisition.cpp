#include "qxrddebug.h"
#include "qxrdacquisition.h"
#include "qcepmutexlocker.h"
#include "qcepallocator.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdwindow.h"
#include "qxrdacqcommon-ptr.h"
#include <QtConcurrentRun>
#include <QDir>
#include <QMetaProperty>
#include "qxrddetectorsettings.h"
#include "qxrdprocessor.h"
#include "qxrdprocessorexecutionthread.h"
#include "qxrdacquisitionparameterpack.h"
#include "qxrddarkacquisitionparameterpack.h"
#include "qxrdacquisitionscalermodel.h"
#include "qxrdappcommon.h"
#include "qxrdacquisitionexecutionthread.h"
#include "qxrdacquisitionexecution.h"
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qxrdexperiment.h"
#include "qxrdsynchronizerthread.h"

QxrdAcquisition::QxrdAcquisition(QString name) :
  inherited(name),
  m_ExecutionThread(),
  m_Mutex(QMutex::Recursive),
  m_SynchronizedAcquisition(NULL),
  m_Idling(1)
{
#ifndef QT_NO_DEBUG
  printf("Constructing acquisition\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisition::QxrdAcquisition(%p)\n", this);
  }

  m_SynchronizedAcquisition =
      QxrdSynchronizedAcquisitionPtr(
        new QxrdSynchronizedAcquisition("synchronizedAcquisition"));

  CONNECT_CHECK(connect(prop_Raw16SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes));
  CONNECT_CHECK(connect(prop_Raw32SaveTime(), &QcepDoubleProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes));
  CONNECT_CHECK(connect(prop_SummedExposures(), &QcepIntProperty::valueChanged,  this, &QxrdAcquisition::updateSaveTimes));
  CONNECT_CHECK(connect(prop_DarkSummedExposures(), &QcepIntProperty::valueChanged, this, &QxrdAcquisition::updateSaveTimes));
  CONNECT_CHECK(connect(prop_ExposureTime(), &QcepDoubleProperty::valueChanged, this, &QxrdAcquisition::onExposureTimeChanged));

  CONNECT_CHECK(connect(&m_IdleTimer, &QTimer::timeout, this, &QxrdAcquisition::onIdleTimeout));

  m_IdleTimer.start(1000);
}

void QxrdAcquisition::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
  
  QxrdAcqCommonPtr acq(
        qSharedPointerDynamicCast<QxrdAcqCommon>(sharedFromThis()));

  m_ExecutionThread =
      QxrdAcquisitionExecutionThreadPtr(
        new QxrdAcquisitionExecutionThread("acquisitionExecutionThread"));

  m_ExecutionThread -> initialize(sharedFromThis());

  m_ExecutionThread -> start();

  m_SynchronizerThread =
      QxrdSynchronizerThreadPtr(
        new QxrdSynchronizerThread("synchronizerThread"));

  m_SynchronizerThread -> initialize(sharedFromThis());

  m_SynchronizerThread -> start();

  m_Synchronizer = m_SynchronizerThread -> synchronizer();

  m_SynchronizedAcquisition -> initialize(sharedFromThis());
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
}

void QxrdAcquisition::registerMetaTypes()
{
  qRegisterMetaType<QxrdAcquisition*>("QxrdAcquisition*");

  QxrdSynchronizedAcquisition::registerMetaTypes();

  qRegisterMetaType<QxrdAcquisitionExecutionThread*>("QxrdAcquisitionExecutionThread*");
  qRegisterMetaType<QxrdAcquisitionExecution*>("QxrdAcquisitionExecution*");
}

void QxrdAcquisition::setupAcquisition()
{
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  if (sync) {
    sync -> setupAcquisition();
  }
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

void QxrdAcquisition::unlock()
{
  m_Acquiring.unlock();
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

void QxrdAcquisition::shutdown()
{
  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdAcquisition::shutdown()");
  }

  for (int i=0; i<detectorCount(); i++) {
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

void QxrdAcquisition::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings);

  if (m_SynchronizedAcquisition) {
    settings->beginGroup("sync");
    m_SynchronizedAcquisition->writeSettings(settings);
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

  inherited::openWindows();

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

  inherited::closeWindows();

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

  int n = settings->beginReadArray("detectors");

  for (int i=0; i<n; i++) {
    splashMessage(tr("Reading detector %1 of %2").arg(i).arg(n));
    settings->setArrayIndex(i);

    QcepObjectPtr obj = QcepObject::readObject(sharedFromThis(), settings);

    if (obj) {
      QxrdDetectorSettingsPtr det =
          qSharedPointerDynamicCast<QxrdDetectorSettings>(obj);

      if (det) {
        det->setObjectName(tr("detectorSettings-%1").arg(i));

        det->set_DetectorNumber(i);

        m_Detectors.append(det);

        QxrdAppCommon *app =
            qobject_cast<QxrdAppCommon*>(g_Application);

        if (app && app->get_StartDetectors() == 0) {
          det->set_Enabled(false);
        }

        det->startOrStop(det->isEnabled());
      }
    }
  }

  settings->endArray();
}

QxrdDetectorSettingsPtr QxrdAcquisition::newDetector(int detType)
{
  if (QThread::currentThread() != thread()) {
    QxrdDetectorSettingsPtr res;
    INVOKE_CHECK(
          QMetaObject::invokeMethod(this,
                                    "newDetector",
                                    Qt::BlockingQueuedConnection,
                                    Q_RETURN_ARG(QxrdDetectorSettingsPtr, res),
                                    Q_ARG(int, detType)));

    return res;
  } else {
    QxrdDetectorSettingsPtr det =
        QxrdDetectorSettings::newDetector(sharedFromThis(), detType);

    return det;
  }
}

void QxrdAcquisition::appendDetector(QxrdDetectorSettingsPtr det)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(this,
                                    "appendDetector",
                                    Qt::BlockingQueuedConnection,
                                    Q_ARG(QxrdDetectorSettingsPtr, det)));
  } else {
    if (det) {
      det -> set_DetectorNumber(m_Detectors.count());

      m_Detectors.append(det);
    }
  }
}

void QxrdAcquisition::clearDetectors()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(this,
                                    "clearDetectors",
                                    Qt::BlockingQueuedConnection));
  } else {
    m_Detectors.resize(0);
  }
}

int QxrdAcquisition::detectorCount() const
{
  return m_Detectors.count();
}

QxrdDetectorSettingsPtr QxrdAcquisition::detector(int n) const
{
  return m_Detectors.value(n);
}

QxrdDetectorSettings *QxrdAcquisition::det(int n) const
{
  return m_Detectors.value(n).data();
}

bool QxrdAcquisition::sanityCheckCommon()
{
  if (detectorCount() == 0) {
    criticalMessage("No Detectors are configured");
    return false;
  }

  int ndet = 0;

  for (int i=0; i<detectorCount(); i++) {
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

  if (det) {
    det->configureDetector();
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

QxrdSynchronizerWPtr QxrdAcquisition::synchronizer()
{
  return m_Synchronizer;
}

QxrdSynchronizedAcquisitionPtr QxrdAcquisition::synchronizedAcquisition() const
{
  return m_SynchronizedAcquisition;
}

void QxrdAcquisition::onIdleTimeout()
{
  if (m_Idling.fetchAndAddOrdered(0)) {
    QTime tic;
    tic.start();

    for (int i=0; i<detectorCount(); i++) {
      QxrdDetectorSettingsPtr det = detector(i);

      if (det && det->isEnabled()) {
        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          printMessage(tr("Det %1 is enabled after %2 msec").arg(i).arg(tic.restart()));
        }

        QcepImageDataBasePtr res = det -> acquireFrameIfAvailable();
        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          printMessage(tr("Tried to acquire frame from det %1 after %2 msec").arg(i).arg(tic.restart()));
        }

        QxrdProcessorExecutionThreadPtr proc = det->processorExecutionThread();

        if (proc) {
//          proc->processIdleImage(res);

          INVOKE_CHECK(
                QMetaObject::invokeMethod(proc.data(), "processIdleImage",
                                          Q_ARG(QcepImageDataBasePtr, res)));

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Processed idle frame from det %1 after %2 msec").arg(i).arg(tic.restart()));
          }
        }
      }
    }
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
