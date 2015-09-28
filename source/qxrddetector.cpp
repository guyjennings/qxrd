#include "qxrddetector.h"
#include "qxrddetectorthread.h"
#include "qxrddetectorproxy.h"
#include "qxrddebug.h"
#include "qxrddetectorprocessor.h"
#include <stdio.h>
#include "qcepmutexlocker.h"
#include "qxrddetectorcontrolwindow.h"
#include "qxrdexperiment.h"
#include "qcepimagedata.h"
#include "qxrdacquisition.h"

QxrdDetector::QxrdDetector(QcepSettingsSaverWPtr saver,
                           QxrdExperimentWPtr    expt,
                           QxrdAcquisitionWPtr   acq,
                           int                   detType,
                           int                   detNum,
                           QcepObject           *parent) :
  QcepObject("detector", parent),
  m_Saver(saver),
  m_Experiment(expt),
  m_Acquisition(acq),
  m_Processor(),
  m_DetectorControlWindow(NULL),
  m_NAcquiredImages(),
  m_AcquiredImages("acquired"),
  m_DetectorNumber(QcepSettingsSaverWPtr(), this, "detectorNumber", detNum, "Detector Number"),
  m_DetectorType(saver, this, "detectorType", detType, "Detector Type"),
  m_DetectorTypeName(QcepSettingsSaverWPtr(), this, "detectorTypeName", QxrdDetectorThread::detectorTypeName(detType), "Detector Type Name"),
  m_Enabled(saver, this, "enabled", true, "Is Detector Enabled?"),
  m_DetectorName(saver, this, "detectorName", QxrdDetectorThread::detectorTypeName(detType), "Detector Name"),
  m_NCols(QcepSettingsSaverWPtr(), this, "nCols", 0, "No of detector cols"),
  m_NRows(QcepSettingsSaverWPtr(), this, "nRows", 0, "No of detector rows"),
  m_Extension(saver, this, "extension", "tif", "File extension")
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetector::QxrdDetector(%p)\n", this);
  }

  connect(prop_Enabled(), &QcepBoolProperty::valueChanged,
          this,           &QxrdDetector::startOrStop);
}

QxrdDetector::~QxrdDetector()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetector::~QxrdDetector(%p)\n", this);
  }

  if (m_DetectorControlWindow) {
    m_DetectorControlWindow->deleteLater();
  }
}

void QxrdDetector::initialize()
{
  m_Processor =
      QxrdDetectorProcessorPtr(
        new QxrdDetectorProcessor(m_Saver, m_Experiment, sharedFromThis()));

  m_Processor->initialize();
}

QxrdDetectorProcessorPtr QxrdDetector::processor()
{
  return m_Processor;
}

void QxrdDetector::printMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdDetector::criticalMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->criticalMessage(msg);
  }
}

void QxrdDetector::statusMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->statusMessage(msg);
  }
}

void QxrdDetector::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  if (m_Processor) {
    m_Processor->readSettings(settings, section+"/processor");
  }
}

void QxrdDetector::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

  if (m_Processor) {
    m_Processor->writeSettings(settings, section+"/processor");
  }
}

bool QxrdDetector::isEnabled()
{
  return get_Enabled();
}

void QxrdDetector::startOrStop(bool enabled)
{
  if (enabled) {
    startDetector();
  } else {
    stopDetector();
  }
}

bool QxrdDetector::checkDetectorEnabled()
{
  if (isEnabled()) {
    return true;
  } else {
    criticalMessage("Attempt to use disabled detector");
    return false;
  }
}

void QxrdDetector::startDetector()
{
}

void QxrdDetector::stopDetector()
{
}

void QxrdDetector::onExposureTimeChanged()
{
}

void QxrdDetector::beginAcquisition(double exposure)
{
}

void QxrdDetector::endAcquisition()
{
}

void QxrdDetector::shutdownAcquisition()
{
}

QScriptValue QxrdDetector::toScriptValue(QScriptEngine *engine, const QxrdDetectorPtr &det)
{
  return engine->newQObject(det.data());
}

void QxrdDetector::fromScriptValue(const QScriptValue &obj, QxrdDetectorPtr &det)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdDetector *qdet = qobject_cast<QxrdDetector*>(qobj);

    if (qdet) {
      det = QxrdDetectorPtr(qdet);
    }
  }
}

void QxrdDetector::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy, int detType)
{
  proxy->clearProperties();

  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::DetectorNumberProperty, "detectorNumber", "Detector Number",  -1);
    proxy->pushProperty(QxrdDetectorProxy::DetectorTypeProperty,   "detectorType",   "Detector Type",     detType);
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty,        "enabled",        "Detector Enabled?", true);
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,         "detectorName",   "Detector Name",     "A " + QxrdDetectorThread::detectorTypeName(detType));
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,         "extension",      "File extension",    "tif");
  }
}

void QxrdDetector::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  proxy->clearProperties();

  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::DetectorNumberProperty, "detectorNumber", "Detector Number",   get_DetectorNumber());
    proxy->pushProperty(QxrdDetectorProxy::DetectorTypeProperty,   "detectorType",   "Detector Type",     get_DetectorType());
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty,        "enabled",        "Detector Enabled?", get_Enabled());
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,         "detectorName",   "Detector Name",     get_DetectorName());
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,         "extension",      "File extension",    get_Extension());
  }
}

void QxrdDetector::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  if (proxy) {
    set_Enabled(proxy->property("enabled").toBool());
    set_DetectorNumber(proxy->property("detectorNumber").toInt());
    set_DetectorName(proxy->property("detectorName").toString());
    set_Extension(proxy->property("extension").toString());
  }
}

void QxrdDetector::openControlWindow()
{
  GUI_THREAD_CHECK;

  if (m_DetectorControlWindow == NULL) {
    m_DetectorControlWindow =
        new QxrdDetectorControlWindow(m_Saver, m_Experiment, m_Acquisition, m_Processor, NULL);
  }

  if (m_DetectorControlWindow) {
    m_DetectorControlWindow->show();
    m_DetectorControlWindow->raise();
  }
}

void QxrdDetector::enqueueAcquiredFrame(QcepImageDataBasePtr img)
{
  m_AcquiredImages.enqueue(img);

  m_NAcquiredImages.release(1);
}

void QxrdDetector::beginFrame()
{
}

QcepImageDataBasePtr QxrdDetector::acquireFrame()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    while (1) {
      if (m_NAcquiredImages.tryAcquire(1, 100)) {
        return m_AcquiredImages.dequeue();
      } else if (acq->get_Cancelling()) {
        return QcepImageDataBasePtr();
      }
    }
  }

  return QcepImageDataBasePtr();
}

QcepImageDataBasePtr QxrdDetector::acquireFrameIfAvailable()
{
  QcepImageDataBasePtr res;

  while (m_NAcquiredImages.available() >= 1) {
    res = acquireFrame();
  }

  return res;
}
