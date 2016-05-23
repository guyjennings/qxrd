#include "qxrddetectorsettings.h"
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
#include "qxrdroicalculator.h"

QxrdDetectorSettings::QxrdDetectorSettings(QxrdExperimentWPtr    expt,
                           QxrdAcquisitionWPtr   acq,
                           int                   detType,
                           int                   detNum) :
  QcepSerializableObject("detector"),
  m_Experiment(expt),
  m_Acquisition(acq),
  m_Processor(),
  m_DetectorControlWindow(NULL),
  m_NAcquiredImages(),
  m_AcquiredImages("acquired"),
  m_DetectorNumber(this, "detectorNumber", detNum, "Detector Number"),
  m_DetectorType(this, "detectorType", detType, "Detector Type"),
  m_DetectorTypeName(this, "detectorTypeName", QxrdDetectorThread::detectorTypeName(detType), "Detector Type Name"),
  m_Enabled(this, "enabled", true, "Is Detector Enabled?"),
  m_DetectorName(this, "detectorName", QxrdDetectorThread::detectorTypeName(detType), "Detector Name"),
  m_NCols(this, "nCols", 0, "No of detector cols"),
  m_NRows(this, "nRows", 0, "No of detector rows"),
  m_HBinning(this, "hBinning", 0, "Horiz Binning"),
  m_VBinning(this, "vBinning", 0, "Vert Binning"),
  m_Extension(this, "extension", "tif", "File extension")
{
#ifndef QT_DEBUG
  printf("Constructing detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetector::QxrdDetector(%p)\n", this);
  }

  QxrdAcquisitionPtr acqs(m_Acquisition);

  if (acqs) {
    acqs->prop_DetectorCount()->incValue(1);
  }
}

void QxrdDetectorSettings::initialize()
{
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

  QxrdAcquisitionPtr a(m_Acquisition);

  if (a) {
    connect(prop_Enabled(), &QcepBoolProperty::valueChanged,
            a.data(), &QxrdAcquisition::detectorStateChanged);
  }
}

QxrdDetectorSettings::~QxrdDetectorSettings()
{
#ifndef QT_DEBUG
  printf("Deleting detector\n");
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

void QxrdDetectorSettings::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  if (m_Processor) {
    m_Processor->readSettings(settings, section+"/processor");
  }
}

void QxrdDetectorSettings::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

  if (m_Processor) {
    m_Processor->writeSettings(settings, section+"/processor");
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
}

void QxrdDetectorSettings::stopDetector()
{
}

void QxrdDetectorSettings::onExposureTimeChanged()
{
}

void QxrdDetectorSettings::beginAcquisition(double exposure)
{
}

void QxrdDetectorSettings::endAcquisition()
{
}

void QxrdDetectorSettings::shutdownAcquisition()
{
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
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty,        "enabled",        "Detector Enabled?", true);
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,         "detectorName",   "Detector Name",     "A " + QxrdDetectorThread::detectorTypeName(detType));
//    proxy->pushProperty(QxrdDetectorProxy::ExtensionProperty,      "extension",      "File extension",    "tif");
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

void QxrdDetectorSettings::openControlWindow()
{
  GUI_THREAD_CHECK;

  if (m_DetectorControlWindow == NULL) {
    m_DetectorControlWindow =
        QxrdDetectorControlWindowPtr(
          new QxrdDetectorControlWindow(m_Experiment,
                                        m_Acquisition,
                                        qSharedPointerDynamicCast<QxrdDetectorSettings>(sharedFromThis()),
                                        m_Processor, NULL));

    QxrdDetectorProcessorPtr dp(m_Processor);

    if (dp) {
      dp->setControlWindow(m_DetectorControlWindow);
    }
  }

  if (m_DetectorControlWindow) {
    m_DetectorControlWindow->show();
    m_DetectorControlWindow->raise();
  }
}

void QxrdDetectorSettings::enqueueAcquiredFrame(QcepImageDataBasePtr img)
{
  m_AcquiredImages.enqueue(img);

  m_NAcquiredImages.release(1);
}

void QxrdDetectorSettings::beginFrame()
{
}

QcepImageDataBasePtr QxrdDetectorSettings::acquireFrame()
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
