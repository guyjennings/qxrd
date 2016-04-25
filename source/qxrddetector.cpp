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
#include "qxrdroicalculator.h"

QxrdDetector::QxrdDetector(QcepSettingsSaverWPtr saver,
                           QxrdExperimentWPtr    expt,
                           QxrdAcquisitionWPtr   acq,
                           int                   detType,
                           int                   detNum,
                           QcepObjectWPtr        parent) :
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
  m_HBinning(QcepSettingsSaverWPtr(), this, "hBinning", 0, "Horiz Binning"),
  m_VBinning(QcepSettingsSaverWPtr(), this, "vBinning", 0, "Vert Binning"),
  m_Extension(saver, this, "extension", "tif", "File extension")
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

void QxrdDetector::initialize()
{
  connect(prop_Enabled(), &QcepBoolProperty::valueChanged,
          this,           &QxrdDetector::startOrStop);

  QxrdExperimentPtr exper(m_Experiment);

  if (exper) {
    m_Processor =
        QxrdDetectorProcessorPtr(
          new QxrdDetectorProcessor(m_Saver, m_Experiment, exper->fileSaver(),
                                    qSharedPointerDynamicCast<QxrdDetector>(sharedFromThis())));
  }

  QxrdAcquisitionPtr a(m_Acquisition);

  if (a) {
    connect(prop_Enabled(), &QcepBoolProperty::valueChanged,
            a.data(), &QxrdAcquisition::detectorStateChanged);
  }
}

QxrdDetector::~QxrdDetector()
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

QxrdExperimentWPtr QxrdDetector::experiment()
{
  return m_Experiment;
}

QxrdAcquisitionWPtr QxrdDetector::acquisition()
{
  return m_Acquisition;
}

QxrdDetectorProcessorPtr QxrdDetector::processor()
{
  return m_Processor;
}

int QxrdDetector::roiCount()
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

QxrdROICoordinatesPtr QxrdDetector::roi(int i)
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

void QxrdDetector::printMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdDetector::criticalMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->criticalMessage(msg);
  }
}

void QxrdDetector::statusMessage(QString msg, QDateTime ts) const
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

QScriptValue QxrdDetector::toScriptValue(QScriptEngine *engine, const QxrdDetectorWPtr &det)
{
  return engine->newQObject(det.data());
}

void QxrdDetector::fromScriptValue(const QScriptValue &obj, QxrdDetectorWPtr &det)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdDetector *qdet = qobject_cast<QxrdDetector*>(qobj);

    if (qdet) {
      QxrdDetectorPtr dp(qSharedPointerDynamicCast<QxrdDetector>(qdet->sharedFromThis()));

      if (dp) {
        det = dp;
      }
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
//    proxy->pushProperty(QxrdDetectorProxy::ExtensionProperty,      "extension",      "File extension",    "tif");
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
//    proxy->pushProperty(QxrdDetectorProxy::ExtensionProperty,      "extension",      "File extension",    get_Extension());
  }
}

void QxrdDetector::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  if (proxy) {
    set_Enabled(proxy->property("enabled").toBool());
    set_DetectorNumber(proxy->property("detectorNumber").toInt());
    set_DetectorName(proxy->property("detectorName").toString());
//    set_Extension(proxy->property("extension").toString());
  }
}

void QxrdDetector::openControlWindow()
{
  GUI_THREAD_CHECK;

  if (m_DetectorControlWindow == NULL) {
    m_DetectorControlWindow =
        QxrdDetectorControlWindowPtr(
          new QxrdDetectorControlWindow(m_Saver,
                                        m_Experiment,
                                        m_Acquisition,
                                        qSharedPointerDynamicCast<QxrdDetector>(sharedFromThis()),
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

QcepDoubleVector QxrdDetector::scalerCounts()
{
  if (m_Processor) {
    return m_Processor->get_RoiCounts();
  } else {
    return QcepDoubleVector();
  }
}

double QxrdDetector::scalerCounts(int chan)
{
  if (m_Processor) {
    return m_Processor->get_RoiCounts().value(chan);
  } else {
    return 0;
  }
}
