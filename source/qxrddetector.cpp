#include "qxrddetector.h"
#include "qxrddetectorthread.h"
#include "qxrddetectorproxy.h"
#include "qxrddebug.h"
#include "qxrdacquisitionprocessor.h"
#include <stdio.h>
#include "qcepmutexlocker.h"

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
  m_AcquisitionProcessor(),
  m_DetectorNumber(QcepSettingsSaverWPtr(), this, "detectorNumber", detNum, "Detector Number"),
  m_DetectorType(saver, this, "detectorType", detType, "Detector Type"),
  m_DetectorTypeName(QcepSettingsSaverWPtr(), this, "detectorTypeName", QxrdDetectorThread::detectorTypeName(detType), "Detector Type Name"),
  m_Enabled(saver, this, "enabled", true, "Is Detector Enabled?"),
  m_DetectorName(saver, this, "detectorName", QxrdDetectorThread::detectorTypeName(detType), "Detector Name")
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetector::QxrdDetector(%p)\n", this);
  }
}

QxrdDetector::~QxrdDetector()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetector::~QxrdDetector(%p)\n", this);
  }
}

void QxrdDetector::initialize()
{
  m_AcquisitionProcessor =
      QxrdAcquisitionProcessorPtr(
        new QxrdAcquisitionProcessor(m_Saver, m_Experiment, sharedFromThis()));
}

QxrdAcquisitionProcessorPtr QxrdDetector::acquisitionProcessor()
{
  return m_AcquisitionProcessor;
}

void QxrdDetector::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  if (m_AcquisitionProcessor) {
    m_AcquisitionProcessor->readSettings(settings, section+"/acquisitionProcessor");
  }
}

void QxrdDetector::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

  if (m_AcquisitionProcessor) {
    m_AcquisitionProcessor->writeSettings(settings, section+"/acquisitionProcessor");
  }
}

void QxrdDetector::onExposureTimeChanged()
{
}

void QxrdDetector::setupExposureMenu(QDoubleSpinBox * /*cb*/, double /*initialExposure*/)
{
}

void QxrdDetector::acquire()
{
}

void QxrdDetector::beginAcquisition()
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
  }
}

void QxrdDetector::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  if (proxy) {
    set_Enabled(proxy->property("enabled").toBool());
    set_DetectorNumber(proxy->property("detectorNumber").toInt());
    set_DetectorName(proxy->property("detectorName").toString());
  }
}
