#include "qxrddetectorsettings.h"
#include "qxrddetectordriverthread.h"
#include "qxrddebug.h"
#include "qxrdprocessor.h"
#include <stdio.h>
#include "qcepmutexlocker.h"
#include "qxrddetectorcontrolwindow.h"
#include "qxrddetectorcontrolwindowsettings.h"
#include "qxrdexperiment.h"
#include "qcepimagedata.h"
#include "qxrdacqcommon.h"
#include "qxrdroicalculator.h"
#include "qxrdareadetectorsettings.h"
#include "qxrdfilewatchersettings.h"
#include "qxrdperkinelmersettings.h"
#include "qxrdpilatussettings.h"
#include "qxrdsimulatedsettings.h"
#include "qxrddexelasettings.h"
#include "qxrdexperiment.h"
#include "qxrdapplication.h"
#include "qxrddetectorplugininterface.h"
#include <QThread>

QxrdDetectorSettings::QxrdDetectorSettings(QString name, int detType) :
  inherited(name),
  m_Application(),
  m_Experiment(),
  m_Acquisition(),
  m_Processor(),
  m_DetectorControlWindow(NULL),
  m_NAcquiredImages(),
  m_AcquiredImages(),
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

  m_Processor =
      QxrdProcessorPtr(
        new QxrdProcessor("processor"));

  m_DetectorControlWindowSettings =
      QxrdDetectorControlWindowSettingsPtr(
        new QxrdDetectorControlWindowSettings("controlWindowSettings"));

  m_AcquiredImages =
      QcepImageBaseQueuePtr(
        new QcepImageBaseQueue("acquired"));

  connect(prop_Enabled(), &QcepBoolProperty::valueChanged,
          this,           &QxrdDetectorSettings::startOrStop);
}

void QxrdDetectorSettings::initialize(QcepObjectWPtr parent)
{
  THREAD_CHECK;

  inherited::initialize(parent);

  printf("QxrdDetectorSettings::initialize(%p)\n", this);

  m_Application = QxrdApplication::findApplication(parent);
  m_Experiment  = QxrdExperiment::findExperiment(parent);
  m_Acquisition = QxrdAcqCommon::findAcquisition(parent);

#ifndef QT_NO_DEBUG
  if (m_Application == NULL) {
    printMessage("QxrdDetectorSettings::m_Application == NULL");
  }

  if (m_Experiment == NULL) {
    printMessage("QxrdDetectorSettings::m_Experiment == NULL");
  }

  if (m_Acquisition == NULL) {
    printMessage("QxrdDetectorSettings::m_Acquisition == NULL");
  }
#endif

  QxrdApplicationPtr appp(
        qSharedPointerDynamicCast<QxrdApplication>(m_Application));

  if (appp) {
    m_DetectorPlugin =
        appp->detectorPlugin(get_DetectorType());
  }

  m_Processor                     -> initialize(sharedFromThis());
  m_DetectorControlWindowSettings -> initialize(sharedFromThis());

  QxrdAcqCommonPtr a(m_Acquisition);

  if (a) {
    connect(prop_Enabled(), &QcepBoolProperty::valueChanged,
            a.data(),       &QxrdAcqCommon::detectorStateChanged);
  }

  m_AcquiredImages -> initialize(sharedFromThis());

  m_DetectorDriverThread =
      QxrdDetectorDriverThreadPtr(
        new QxrdDetectorDriverThread(
          qSharedPointerDynamicCast<QxrdDetectorSettings>(sharedFromThis())));

  m_DetectorDriverThread -> initialize(sharedFromThis());

  m_DetectorDriverThread -> start();
}

QxrdDetectorSettingsWPtr QxrdDetectorSettings::findDetectorSettings(QcepObjectWPtr p)
{
  QxrdDetectorSettingsWPtr res =
      qSharedPointerDynamicCast<QxrdDetectorSettings>(p);

  if (res == NULL) {
    QcepObjectPtr objp(p);

    if (objp) {
      res = findDetectorSettings(objp->parentPtr());
    }
  }

  return res;
}

QxrdDetectorSettings::~QxrdDetectorSettings()
{
#ifndef QT_NO_DEBUG
  printf("Deleting detector settings\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetector::~QxrdDetector(%p)\n", this);
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
    res = "noDetector";
    break;

  case Simulated:
    res = "simulatedDetector";
    break;

  case PerkinElmer:
    res = "perkinElmerDetector";
    break;

  case Pilatus:
    res = "pilatusDetector";
    break;

  case AreaDetector:
    res = "epicsAreaDetector";
    break;

  case FileWatcher:
    res = "fileWatcher";
    break;

  case Dexela:
    res = "dexelaDetector";
    break;
  }

  return res;
}

QStringList QxrdDetectorSettings::detectorTypeNames()
{
  QStringList res;

  res.append(detectorTypeName(NoDetector));
  res.append(detectorTypeName(Simulated));
  res.append(detectorTypeName(PerkinElmer));
  res.append(detectorTypeName(Pilatus));
  res.append(detectorTypeName(AreaDetector));
  res.append(detectorTypeName(FileWatcher));
  res.append(detectorTypeName(Dexela));

  return res;
}

QxrdExperimentWPtr QxrdDetectorSettings::experiment()
{
  return m_Experiment;
}

QxrdAcqCommonWPtr QxrdDetectorSettings::acquisition()
{
  return m_Acquisition;
}

QxrdProcessorPtr QxrdDetectorSettings::processor()
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

QxrdROIPtr QxrdDetectorSettings::roi(int i)
{
  QxrdROIPtr res;

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
  inherited::readSettings(settings);

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
  inherited::writeSettings(settings);

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

void QxrdDetectorSettings::startDetector()
{
  if (m_DetectorDriverThread) {
    m_DetectorDriverThread->startDetectorDriver();
  }
}

void QxrdDetectorSettings::stopDetector()
{
  if (m_DetectorDriverThread) {
    m_DetectorDriverThread->stopDetectorDriver();
  }
}

void QxrdDetectorSettings::changeExposureTime(double expos)
{
  if (m_DetectorDriverThread) {
    m_DetectorDriverThread->changeExposureTime(expos);
  }
}

void QxrdDetectorSettings::beginAcquisition(double exposure)
{
  if (m_DetectorDriverThread) {
    m_DetectorDriverThread->beginAcquisition(exposure);
  }
}

void QxrdDetectorSettings::beginFrame()
{
  if (m_DetectorDriverThread) {
    m_DetectorDriverThread->beginFrame();
  }
}

void QxrdDetectorSettings::endAcquisition()
{
  if (m_DetectorDriverThread) {
    m_DetectorDriverThread->endAcquisition();
  }
}

void QxrdDetectorSettings::shutdownAcquisition()
{
  if (m_DetectorDriverThread) {
    m_DetectorDriverThread->shutdownAcquisition();
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

//    QxrdProcessorPtr dp(m_Processor);

//    if (dp) {
//      dp->setControlWindow(m_DetectorControlWindow);
//    }
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
  if (img) {
    img -> setParentPtr(sharedFromThis());

    if (m_AcquiredImages) {
      m_AcquiredImages -> enqueue(img);
    }

    m_NAcquiredImages.release(1);
  }
}

QcepImageDataBasePtr QxrdDetectorSettings::acquireFrame()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq && m_AcquiredImages) {
    while (1) {
      if (m_NAcquiredImages.tryAcquire(1, 1000)) {
        return m_AcquiredImages -> dequeue();
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

int QxrdDetectorSettings::availableImageCount()
{
  return m_NAcquiredImages.available();
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

QxrdDetectorSettingsPtr QxrdDetectorSettings::newDetector(QcepObjectWPtr parent,
                                                          int detType)
{
  QxrdDetectorSettingsPtr det;

  switch (detType) {
  case Simulated:
    det = QxrdDetectorSettingsPtr(
          new QxrdSimulatedSettings("simulated"));
    break;

  case PerkinElmer:
    det = QxrdDetectorSettingsPtr(
          new QxrdPerkinElmerSettings("perkinElmer"));
    break;

  case Pilatus:
    det = QxrdDetectorSettingsPtr(
          new QxrdPilatusSettings("pilatus"));
    break;

  case AreaDetector:
    det = QxrdDetectorSettingsPtr(
          new QxrdAreaDetectorSettings("epicsArea"));
    break;

  case FileWatcher:
    det = QxrdDetectorSettingsPtr(
          new QxrdFileWatcherSettings("fileWatcher"));
    break;

  case Dexela:
    det = QxrdDetectorSettingsPtr(
          new QxrdDexelaSettings("dexela"));
    break;
  }

  if (det == NULL) {
    det = QxrdDetectorSettingsPtr(
          new QxrdSimulatedSettings("simulated"));
  }

  if (det) {
    det -> initialize(parent);
  }

  return det;
}

void QxrdDetectorSettings::configureDetector()
{
  printf("QxrdDetectorSettings::configureDetector is not implemented\n");
}

QxrdDetectorDriverPtr QxrdDetectorSettings::createDetector(
    QString name,
    QxrdDetectorSettingsWPtr det,
    QxrdExperimentWPtr expt,
    QxrdAcqCommonWPtr acq)
{
  QxrdDetectorDriverPtr res;

  QxrdDetectorPluginInterfacePtr plugin(m_DetectorPlugin);

  if (plugin) {
    res = plugin->createDetector(name, det, expt, acq);
  }

  return res;
}

void QxrdDetectorSettings::registerMetaTypes()
{
  qRegisterMetaType<QxrdSimulatedSettings*>("QxrdSimulatedSettings*");
  qRegisterMetaType<QxrdPerkinElmerSettings*>("QxrdPerkinElmerSettings*");
  qRegisterMetaType<QxrdPilatusSettings*>("QxrdPilatusSettings*");
  qRegisterMetaType<QxrdAreaDetectorSettings*>("QxrdAreaDetectorSettings*");
  qRegisterMetaType<QxrdFileWatcherSettings*>("QxrdFileWatcherSettings*");
  qRegisterMetaType<QxrdDexelaSettings*>("QxrdDexelaSettings*");
}
