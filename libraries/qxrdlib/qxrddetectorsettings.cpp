#include "qxrddetectorsettings.h"
#include "qxrddetectordriverthread.h"
#include "qxrddetectorproxy.h"
#include "qxrddebug.h"
#include "qxrddetectorprocessor.h"
#include <stdio.h>
#include "qcepmutexlocker.h"
#include "qxrddetectorcontrolwindow.h"
#include "qxrddetectorcontrolwindowsettings.h"
#include "qxrdexperiment.h"
#include "qcepimagedata.h"
#include "qxrdacquisition.h"
#include "qxrdroicalculator.h"
#include "qxrddetectorsettingsepicsarea.h"
#include "qxrddetectorsettingsfilewatcher.h"
#include "qxrddetectorsettingsperkinelmer.h"
#include "qxrddetectorsettingspilatus.h"
#include "qxrddetectorsettingssimulated.h"
#include "qxrddexelasettings.h"
#include "qxrdexperiment.h"

QxrdDetectorSettings::QxrdDetectorSettings(QString name, int detType) :
  QcepObject(name),
  m_Application(),
  m_Experiment(),
  m_Acquisition(),
  m_Processor(),
  m_DetectorControlWindow(NULL),
  m_NAcquiredImages(),
  m_AcquiredImages("acquired"),
  m_DetectorNumber(this, "detectorNumber", -1, "Detector Number"),
  m_DetectorType(this, "detectorType", detType, "Detector Type"),
  m_DetectorTypeName(this, "detectorTypeName", detectorTypeName(detType), "Detector Type Name"),
  m_Enabled(this, "enabled", true, "Is Detector Enabled?"),
  m_DetectorName(this, "detectorName", detectorTypeName(detType), "Detector Name"),
  m_NCols(this, "nCols", 0, "No of detector cols"),
  m_NRows(this, "nRows", 0, "No of detector rows"),
  m_HBinning(this, "hBinning", 0, "Horiz Binning"),
  m_VBinning(this, "vBinning", 0, "Vert Binning"),
  m_Extension(this, "extension", "tif", "File extension")
{
#ifndef QT_NO_DEBUG
  printf("Constructing detector settings\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorSettings::QxrdDetectorSettings(%p)\n", this);
  }

  QxrdAcquisitionPtr acqs(m_Acquisition);

  if (acqs) {
    acqs->prop_DetectorCount()->incValue(1);
  }
}

void QxrdDetectorSettings::initialize(QxrdApplicationWPtr   app,
                                      QxrdExperimentWPtr    expt,
                                      QxrdAcquisitionWPtr   acq,
                                      int                   detNum)
{
  m_Application = app;
  m_Experiment  = expt;
  m_Acquisition = acq;

  set_DetectorNumber(detNum);


  connect(prop_Enabled(), &QcepBoolProperty::valueChanged,
          this,           &QxrdDetectorSettings::startOrStop);

  QxrdExperimentPtr exper(m_Experiment);

  if (exper) {
    m_Processor =
        QxrdDetectorProcessorPtr(
          new QxrdDetectorProcessor(m_Experiment,
                                    exper->fileSaver(),
                                    qSharedPointerDynamicCast<QxrdDetectorSettings>(sharedFromThis())));
  }

  m_DetectorControlWindowSettings =
      QxrdDetectorControlWindowSettings::newDetectorWindowSettings(
        qSharedPointerDynamicCast<QxrdDetectorSettings>(sharedFromThis()));

  QxrdAcquisitionPtr a(m_Acquisition);

  if (a) {
    connect(prop_Enabled(), &QcepBoolProperty::valueChanged,
            a.data(), &QxrdAcquisition::detectorStateChanged);
  }

  m_DetectorDriver =
      QxrdDetectorDriverThread::newDetectorDriverThread(
        qSharedPointerDynamicCast<QxrdDetectorSettings>(sharedFromThis()));

  m_DetectorDriver -> start();
}

QxrdDetectorSettings::~QxrdDetectorSettings()
{
#ifndef QT_NO_DEBUG
  printf("Deleting detector settings\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetector::~QxrdDetector(%p)\n", this);
  }

//  if (m_DetectorControlWindow) {
//    m_DetectorControlWindow->deleteLater();
//  }
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq->prop_DetectorCount()->incValue(-1);
  }
}

void QxrdDetectorSettings::printLine(QString line) const
{
  if (m_DetectorControlWindow) {
    m_DetectorControlWindow->printLine(line);
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->printLine(line);
  } else {
    printf("%s\n", qPrintable(line));
  }
}

void QxrdDetectorSettings::printMessage(QString msg, QDateTime dt) const
{
  if (m_DetectorControlWindow) {
    m_DetectorControlWindow->printMessage(msg, dt);
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->printMessage(msg, dt);
  } else {
    printf("MESSAGE: %s\n", qPrintable(msg));
  }
}

void QxrdDetectorSettings::criticalMessage(QString msg, QDateTime ts) const
{
  if (m_DetectorControlWindow) {
    m_DetectorControlWindow->criticalMessage(msg, ts);
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->criticalMessage(msg, ts);
  } else {
    printf("CRITICAL: %s\n", qPrintable(msg));
  }
}

void QxrdDetectorSettings::statusMessage(QString msg, QDateTime ts) const
{
  if (m_DetectorControlWindow) {
    m_DetectorControlWindow->statusMessage(msg, ts);
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->statusMessage(msg, ts);
  } else {
    printf("STATUS: %s\n", qPrintable(msg));
  }
}

int QxrdDetectorSettings::detectorTypeCount()
{
  return 7;
}

QString QxrdDetectorSettings::detectorTypeName(int detectorType)
{
  QString res = "unknown";
  switch (detectorType) {
  case NoDetector:
    res = "No Detector";
    break;

  case SimulatedDetector:
    res = "Simulated Detector";
    break;

  case PerkinElmerDetector:
    res = "Perkin Elmer Detector";
    break;

  case PilatusDetector:
    res = "Pilatus Detector";
    break;

  case EpicsAreaDetector:
    res = "Epics Area Detector";
    break;

  case FileWatcherDetector:
    res = "File Watcher";
    break;

  case DexelaDetector:
    res = "Dexela Detector";
    break;
  }

  return res;
}

QStringList QxrdDetectorSettings::detectorTypeNames()
{
  QStringList res;

  res.append(detectorTypeName(NoDetector));
  res.append(detectorTypeName(SimulatedDetector));
  res.append(detectorTypeName(PerkinElmerDetector));
  res.append(detectorTypeName(PilatusDetector));
  res.append(detectorTypeName(EpicsAreaDetector));
  res.append(detectorTypeName(FileWatcherDetector));
  res.append(detectorTypeName(DexelaDetector));

  return res;
}

QxrdExperimentWPtr QxrdDetectorSettings::experiment()
{
  return m_Experiment;
}

QxrdAcquisitionWPtr QxrdDetectorSettings::acquisition()
{
  return m_Acquisition;
}

QxrdDetectorProcessorPtr QxrdDetectorSettings::processor()
{
  return m_Processor;
}

QxrdDetectorControlWindowSettingsWPtr QxrdDetectorSettings::detectorControlWindowSettings()
{
  return m_DetectorControlWindowSettings;
}

QxrdDetectorControlWindowWPtr QxrdDetectorSettings::detectorControlWindow()
{
  return m_DetectorControlWindow;
}

int QxrdDetectorSettings::roiCount()
{
  int res = 0;

  if (m_Processor) {
    QxrdROICalculatorPtr calc = m_Processor->roiCalculator();

    if (calc) {
      res = calc->roiCount();
    }
  }

  return res;
}

QxrdROICoordinatesPtr QxrdDetectorSettings::roi(int i)
{
  QxrdROICoordinatesPtr res;

  if (m_Processor) {
    QxrdROICalculatorPtr calc = m_Processor->roiCalculator();

    if (calc) {
      res = calc->roi(i);
    }
  }

  return res;
}

void QxrdDetectorSettings::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings);

  if (m_Processor) {
    settings->beginGroup("processor");
    m_Processor->readSettings(settings);
    settings->endGroup();
  }

  if (m_DetectorControlWindowSettings) {
    settings->beginGroup("windowSettings");
    m_DetectorControlWindowSettings->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdDetectorSettings::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings);

  if (m_Processor) {
    settings->beginGroup("processor");
    m_Processor->writeSettings(settings);
    settings->endGroup();
  }

  if (m_DetectorControlWindow) {
    m_DetectorControlWindow->captureSize();
  }

  if (m_DetectorControlWindowSettings) {
    settings->beginGroup("windowSettings");
    m_DetectorControlWindowSettings->writeSettings(settings);
    settings->endGroup();
  }
}

bool QxrdDetectorSettings::isEnabled()
{
  return get_Enabled();
}

void QxrdDetectorSettings::startOrStop(bool enabled)
{
  if (enabled) {
    startDetector();
  } else {
    stopDetector();
  }
}

bool QxrdDetectorSettings::checkDetectorEnabled()
{
  if (isEnabled()) {
    return true;
  } else {
    criticalMessage("Attempt to use disabled detector");
    return false;
  }
}

bool QxrdDetectorSettings::startDetector()
{
  if (m_DetectorDriver) {
    return m_DetectorDriver->startDetectorDriver();
  } else {
    return false;
  }
}

bool QxrdDetectorSettings::stopDetector()
{
  if (m_DetectorDriver) {
    return m_DetectorDriver->stopDetectorDriver();
  } else {
    return false;
  }
}

bool QxrdDetectorSettings::changeExposureTime(double expos)
{
  if (m_DetectorDriver) {
    return m_DetectorDriver->changeExposureTime(expos);
  } else {
    return false;
  }
}

bool QxrdDetectorSettings::beginAcquisition(double exposure)
{
  if (m_DetectorDriver) {
    return m_DetectorDriver->beginAcquisition(exposure);
  } else {
    return false;
  }
}

void QxrdDetectorSettings::beginFrame()
{
  if (m_DetectorDriver) {
    m_DetectorDriver->beginFrame();
  }
}

bool QxrdDetectorSettings::endAcquisition()
{
  if (m_DetectorDriver) {
    return m_DetectorDriver->endAcquisition();
  } else {
    return false;
  }
}

bool QxrdDetectorSettings::shutdownAcquisition()
{
  if (m_DetectorDriver) {
    return m_DetectorDriver->shutdownAcquisition();
  } else {
    return false;
  }
}

QScriptValue QxrdDetectorSettings::toScriptValue(QScriptEngine *engine, const QxrdDetectorSettingsWPtr &det)
{
  return engine->newQObject(det.data());
}

void QxrdDetectorSettings::fromScriptValue(const QScriptValue &obj, QxrdDetectorSettingsWPtr &det)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdDetectorSettings *qdet = qobject_cast<QxrdDetectorSettings*>(qobj);

    if (qdet) {
      QxrdDetectorSettingsPtr dp(qSharedPointerDynamicCast<QxrdDetectorSettings>(qdet->sharedFromThis()));

      if (dp) {
        det = dp;
      }
    }
  }
}

void QxrdDetectorSettings::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy, int detType)
{
  proxy->clearProperties();

  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::DetectorNumberProperty, "detectorNumber", "Detector Number",  -1);
    proxy->pushProperty(QxrdDetectorProxy::DetectorTypeProperty,   "detectorType",   "Detector Type",     detType);
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty,        "enabled",        "Detector Enabled?", false);
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,         "detectorName",   "Detector Name",     "A " + detectorTypeName(detType));
    //    proxy->pushProperty(QxrdDetectorProxy::ExtensionProperty,      "extension",      "File extension",    "tif");

    switch (detType) {
    case NoDetector:
    default:
      break;

    case SimulatedDetector:
      QxrdDetectorSettingsSimulated::pushDefaultsToProxy(proxy);
      break;

    case PerkinElmerDetector:
      QxrdDetectorSettingsPerkinElmer::pushDefaultsToProxy(proxy);
      break;

    case PilatusDetector:
      QxrdDetectorSettingsPilatus::pushDefaultsToProxy(proxy);
      break;

    case EpicsAreaDetector:
      QxrdDetectorSettingsEpicsArea::pushDefaultsToProxy(proxy);
      break;

    case FileWatcherDetector:
      QxrdDetectorSettingsFileWatcher::pushDefaultsToProxy(proxy);
      break;

    case DexelaDetector:
      QxrdDexelaSettings::pushDefaultsToProxy(proxy);
      break;
    }
  }
}

void QxrdDetectorSettings::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  proxy->clearProperties();

  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::DetectorNumberProperty, "detectorNumber", "Detector Number",   get_DetectorNumber());
    proxy->pushProperty(QxrdDetectorProxy::DetectorTypeProperty,   "detectorType",   "Detector Type",     get_DetectorType());
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty,        "enabled",        "Detector Enabled?", get_Enabled());
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,         "detectorName",   "Detector Name",     get_DetectorName());
//    proxy->pushProperty(QxrdDetectorProxy::ExtensionProperty,      "extension",      "File extension",    get_Extension());
  }
}

void QxrdDetectorSettings::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  if (proxy) {
    set_Enabled(proxy->property("enabled").toBool());
    set_DetectorNumber(proxy->property("detectorNumber").toInt());
    set_DetectorName(proxy->property("detectorName").toString());
//    set_Extension(proxy->property("extension").toString());
  }
}

void QxrdDetectorSettings::openWindow()
{
  GUI_THREAD_CHECK;

  QxrdDetectorControlWindowSettingsPtr set(detectorControlWindowSettings());

  if (set && set->get_DetectorWindowOpen()) {
    openControlWindow();
  }
}

void QxrdDetectorSettings::openControlWindow()
{
  GUI_THREAD_CHECK;

  if (m_DetectorControlWindow == NULL) {
    m_DetectorControlWindow =
        QxrdDetectorControlWindowPtr(
          new QxrdDetectorControlWindow(m_Application,
                                        m_Experiment,
                                        m_Acquisition,
                                        qSharedPointerDynamicCast<QxrdDetectorSettings>(sharedFromThis()),
                                        m_Processor, NULL));

    QxrdDetectorControlWindowSettingsPtr set(detectorControlWindowSettings());

    if (set && m_DetectorControlWindow) {
      QByteArray geometry = set->get_DetectorWindowGeometry();
      QByteArray winstate = set->get_DetectorWindowState();

      if (!m_DetectorControlWindow->restoreGeometry(geometry)) {
        printf("Restore geometry failed\n");
      }

      if (!m_DetectorControlWindow->restoreState(winstate, QXRD_DETECTOR_WINDOW_STATE_VERSION)) {
        printf("Restore state failed\n");
      }
    }

    QxrdDetectorProcessorPtr dp(m_Processor);

    if (dp) {
      dp->setControlWindow(m_DetectorControlWindow);
    }
  }

  if (m_DetectorControlWindow) {
    m_DetectorControlWindow->show();
    m_DetectorControlWindow->raise();
    m_DetectorControlWindow->activateWindow();
  }
}

void QxrdDetectorSettings::closeWindow()
{
  GUI_THREAD_CHECK;

  QxrdDetectorControlWindowSettingsPtr set(detectorControlWindowSettings());

  if (set) {
    set->set_DetectorWindowOpen(m_DetectorControlWindow != NULL);

    if (m_DetectorControlWindow) {
      QRect geom = m_DetectorControlWindow->geometry();

      set->set_DetectorWindowRect(geom);

      m_DetectorControlWindow = QxrdDetectorControlWindowPtr();
    }
  }
}

void QxrdDetectorSettings::enqueueAcquiredFrame(QcepImageDataBasePtr img)
{
  m_AcquiredImages.enqueue(img);

  m_NAcquiredImages.release(1);
}

QcepImageDataBasePtr QxrdDetectorSettings::acquireFrame()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    while (1) {
      if (m_NAcquiredImages.tryAcquire(1, 1000)) {
        return m_AcquiredImages.dequeue();
      } else if (acq->get_Cancelling()) {
        return QcepImageDataBasePtr();
      }
    }
  }

  return QcepImageDataBasePtr();
}

QcepImageDataBasePtr QxrdDetectorSettings::acquireFrameIfAvailable()
{
  QcepImageDataBasePtr res;

  while (m_NAcquiredImages.available() >= 1) {
    res = acquireFrame();
  }

  return res;
}

QcepDoubleVector QxrdDetectorSettings::scalerCounts()
{
  if (m_Processor) {
    return m_Processor->get_RoiCounts();
  } else {
    return QcepDoubleVector();
  }
}

double QxrdDetectorSettings::scalerCounts(int chan)
{
  if (m_Processor) {
    return m_Processor->get_RoiCounts().value(chan);
  } else {
    return 0;
  }
}

QxrdDetectorSettingsPtr QxrdDetectorSettings::newDetector(QxrdApplicationWPtr app,
                                                          QxrdExperimentWPtr expt,
                                                          QxrdAcquisitionWPtr acq,
                                                          int detType,
                                                          int detNum)
{
  QxrdDetectorSettingsPtr det;

  switch (detType) {
  case SimulatedDetector:
    det = QxrdDetectorSettingsPtr(
          new QxrdDetectorSettingsSimulated(tr("simulated-%1").arg(detNum)));
    break;

  case PerkinElmerDetector:
    det = QxrdDetectorSettingsPtr(
          new QxrdDetectorSettingsPerkinElmer(tr("perkinElmer-%1").arg(detNum)));
    break;

  case PilatusDetector:
    det = QxrdDetectorSettingsPtr(
          new QxrdDetectorSettingsPilatus(tr("pilatus-%1").arg(detNum)));
    break;

  case EpicsAreaDetector:
    det = QxrdDetectorSettingsPtr(
          new QxrdDetectorSettingsEpicsArea(tr("epicsArea-%1").arg(detNum)));
    break;

  case FileWatcherDetector:
    det = QxrdDetectorSettingsPtr(
          new QxrdDetectorSettingsFileWatcher(tr("fileWatcher-%1").arg(detNum)));
    break;

  case DexelaDetector:
    det = QxrdDetectorSettingsPtr(
          new QxrdDexelaSettings(tr("dexela-%1").arg(detNum)));
    break;
  }

  if (det == NULL) {
    det = QxrdDetectorSettingsPtr(
          new QxrdDetectorSettingsSimulated(tr("simulated-%1").arg(detNum)));
  }

  if (det) {
    det -> initialize(app, expt, acq, detNum);
  }

  return det;
}

void QxrdDetectorSettings::configureDetector()
{
  printf("QxrdDetectorSettings::configureDetector is not implemented\n");
}

void QxrdDetectorSettings::registerMetaTypes()
{
  qRegisterMetaType<QxrdDetectorSettingsSimulated*>("QxrdDetectorSettingsSimulated*");
  qRegisterMetaType<QxrdDetectorSettingsPerkinElmer*>("QxrdDetectorSettingsPerkinElmer*");
  qRegisterMetaType<QxrdDetectorSettingsPilatus*>("QxrdDetectorSettingsPilatus*");
  qRegisterMetaType<QxrdDetectorSettingsEpicsArea*>("QxrdDetectorSettingsEpicsArea*");
  qRegisterMetaType<QxrdDetectorSettingsFileWatcher*>("QxrdDetectorSettingsFileWatcher*");
  qRegisterMetaType<QxrdDexelaSettings*>("QxrdDexelaSettings*");
}
