#include "qxrddetector.h"
#include "qxrddetectorthread.h"
#include "qxrddetectorproxy.h"
#include "qxrddebug.h"
#include <stdio.h>

QxrdDetector::QxrdDetector(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detType, QcepObject *parent) :
  QcepObject("detector", parent),
  m_Saver(saver),
  m_Experiment(expt),
  m_Acquisition(acq),
  m_DetectorType(saver, this, "detectorType", detType, "Detector Type"),
  m_DetectorTypeName(QcepSettingsSaverWPtr(), this, "detectorTypeName", detectorTypeName(), "Detector Type Name"),
  m_Enabled(saver, this, "enabled", true, "Is Detector Enabled?"),
  m_DetectorName(saver, this, "detectorName", detectorTypeName(), "Detector Name")
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

int  QxrdDetector::detectorType() const
{
  return get_DetectorType();
}

QString QxrdDetector::detectorTypeName() const
{
  return QxrdDetectorThread::detectorTypeName(detectorType());
}

int QxrdDetector::detectorSubType() const
{
  return QxrdDetectorThread::PCI_SubType;
}

QString QxrdDetector::detectorAddress() const
{
  return "";
}

void QxrdDetector::acquire()
{
}

void QxrdDetector::initialize()
{
}

void QxrdDetector::onExposureTimeChanged()
{
}

void QxrdDetector::onBinningModeChanged()
{
}

void QxrdDetector::onCameraGainChanged()
{
}

void QxrdDetector::setupExposureMenu(QDoubleSpinBox * /*cb*/, double /*initialExposure*/)
{
}

void QxrdDetector::setupCameraGainMenu(QComboBox * /*cb*/, int /*initialGain*/)
{
}

void QxrdDetector::setupCameraBinningModeMenu(QComboBox * /*cb*/, int /*initialBinning*/)
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
  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::DetectorTypeProperty, "detectorType", "Detector Type",     detType);
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty,      "enabled",      "Detector Enabled?", true);
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,       "detectorName", "Detector Name",     "A " + QxrdDetectorThread::detectorTypeName(detType));
  }
}

void QxrdDetector::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::DetectorTypeProperty, "detectorType", "Detector Type",     get_DetectorType());
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty,      "enabled",      "Detector Enabled?", get_Enabled());
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,       "detectorName", "Detector Name",     get_DetectorName());
  }
}

void QxrdDetector::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  if (proxy) {
    set_Enabled(proxy->property("enabled").toBool());
    set_DetectorName(proxy->property("detectorName").toString());
  }
}
