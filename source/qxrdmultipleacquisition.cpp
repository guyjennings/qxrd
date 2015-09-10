#include "qxrdmultipleacquisition.h"
#include "qxrddetectorthread.h"
#include "qxrddetectorproxy.h"
#include "qxrddebug.h"

QxrdMultipleAcquisition::QxrdMultipleAcquisition(QcepSettingsSaverWPtr saver,
                                                 QxrdExperimentWPtr    doc,
                                                 QxrdDataProcessorWPtr proc,
                                                 QcepAllocatorWPtr     allocator)
      : QxrdAcquisition(saver, doc, proc, allocator),
        m_DetectorCount(m_Saver, this, "detectorCount", 0, "Number of Detectors")
{

}

QxrdMultipleAcquisition::~QxrdMultipleAcquisition()
{
#ifndef QT_NO_DEBUG
  printf("Deletint multiple acquisition\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdMultipleAcquisition::~QxrdMultipleAcquisition(%p)\n", this);
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdMultipleAcquisition::~QxrdMultipleAcquisition");
  }
}

void QxrdMultipleAcquisition::readSettings(QSettings *settings, QString section)
{
  QxrdAcquisition::readSettings(settings, section);

  if (get_DetectorCount() > 0) {
    int n = settings->beginReadArray(section+"/detectors");

    m_DetectorThreads.resize(n);
    m_Detectors.resize(n);

    for (int i=0; i<n; i++) {
      settings->setArrayIndex(i);

      int detType = settings->value("detectorType", 0).toInt();

      QxrdDetectorThreadPtr detThread =
          QxrdDetectorThreadPtr(new QxrdDetectorThread(m_Saver, experiment(), sharedFromThis(), detType, this));

      if (detThread) {
        detThread->start();

        QxrdDetectorPtr det = detThread->detector();

        if (det) {
          det->readSettings(settings, "");

          m_DetectorThreads[i] = detThread;
          m_Detectors[i]       = det;
        }
      }
    }

    settings->endArray();
  }
}

void QxrdMultipleAcquisition::writeSettings(QSettings *settings, QString section)
{
  QxrdAcquisition::writeSettings(settings, section);

  if (get_DetectorCount() > 0) {
    settings->beginWriteArray(section+"/detectors");

    for (int i=0; i<get_DetectorCount(); i++) {
      settings->setArrayIndex(i);

      m_Detectors[i]->writeSettings(settings, "");
    }

    settings->endArray();
  }
}

void QxrdMultipleAcquisition::appendDetector(int detType)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "appendDetector", Qt::BlockingQueuedConnection,
                              Q_ARG(int, detType));
  } else {
    QxrdDetectorThreadPtr detThread =
        QxrdDetectorThreadPtr(new QxrdDetectorThread(m_Saver, experiment(), sharedFromThis(), detType, this));

    if (detThread) {
      detThread->start();

      QxrdDetectorPtr det = detThread->detector();

      if (det) {
        m_DetectorThreads.append(detThread);
        m_Detectors.append(det);

        set_DetectorCount(m_Detectors.count());
      }
    }
  }
}

void QxrdMultipleAcquisition::appendDetectorProxy(QxrdDetectorProxyPtr proxy)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "appendDetectorProxy", Qt::BlockingQueuedConnection,
                              Q_ARG(QxrdDetectorProxyPtr, proxy));
  } else {
    if (proxy) {
      QxrdDetectorThreadPtr detThread = proxy->detectorThread();
      QxrdDetectorPtr       detector  = proxy->detector();

      if (detThread && detector) {
        m_DetectorThreads.append(detThread);
        m_Detectors.append(detector);

        set_DetectorCount(m_Detectors.count());
      } else {
        int detType = proxy->detectorType();

        appendDetector(detType);
      }
    }
  }
}

void QxrdMultipleAcquisition::clearDetectors()
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "clearDetectors", Qt::BlockingQueuedConnection);
  } else {
    m_Detectors.resize(0);

    set_DetectorCount(0);
  }
}

QxrdDetectorThreadPtr QxrdMultipleAcquisition::detectorThread(int n)
{
  return m_DetectorThreads.value(n);
}

QxrdDetectorPtr QxrdMultipleAcquisition::detector(int n)
{
  return m_Detectors.value(n);
}

void QxrdMultipleAcquisition::onExposureTimeChanged()
{
  foreach (QxrdDetectorPtr det, m_Detectors) {
    det->onExposureTimeChanged();
  }
}

void QxrdMultipleAcquisition::onBinningModeChanged()
{
  foreach (QxrdDetectorPtr det, m_Detectors) {
    det->onBinningModeChanged();
  }
}

void QxrdMultipleAcquisition::onCameraGainChanged()
{
  foreach (QxrdDetectorPtr det, m_Detectors) {
    det->onCameraGainChanged();
  }
}

void QxrdMultipleAcquisition::setupExposureMenu(QDoubleSpinBox *cb)
{
}

void QxrdMultipleAcquisition::setupCameraGainMenu(QComboBox *cb)
{
}

void QxrdMultipleAcquisition::setupCameraBinningModeMenu(QComboBox *cb)
{
}

void QxrdMultipleAcquisition::beginAcquisition()
{
  foreach (QxrdDetectorPtr det, m_Detectors) {
    det->beginAcquisition();
  }
}

void QxrdMultipleAcquisition::endAcquisition()
{
  foreach (QxrdDetectorPtr det, m_Detectors) {
    det->endAcquisition();
  }
}

void QxrdMultipleAcquisition::shutdownAcquisition()
{
  foreach (QxrdDetectorPtr det, m_Detectors) {
    det->shutdownAcquisition();
  }
}

void QxrdMultipleAcquisition::acquire()
{
  foreach (QxrdDetectorPtr det, m_Detectors) {
    det->acquire();
  }

  prop_FileIndex()->incValue(get_PostTriggerFiles());
}

void QxrdMultipleAcquisition::configureDetector(int i)
{
  printMessage(tr("Configure Detector %1").arg(i));

  QxrdDetectorPtr det = detector(i);

  QxrdDetectorProxyPtr proxy(new QxrdDetectorProxy(detectorThread(i), detector(i), sharedFromThis()));

  proxy->configureDetector();
}
