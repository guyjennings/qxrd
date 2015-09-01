#include "qxrdmultipleacquisition.h"
#include "qxrddetectorthread.h"

QxrdMultipleAcquisition::QxrdMultipleAcquisition(QcepSettingsSaverWPtr saver,
                                                 QxrdExperimentWPtr    doc,
                                                 QxrdDataProcessorWPtr proc,
                                                 QcepAllocatorWPtr     allocator)
      : QxrdAcquisition(saver, doc, proc, allocator),
        m_DetectorCount(m_Saver, this, "detectorCount", 0, "Number of Detectors")
{

}

void QxrdMultipleAcquisition::readSettings(QSettings *settings, QString section)
{
  QxrdAcquisition::readSettings(settings, section);

  if (get_DetectorCount() > 0) {
    int n = settings->beginReadArray(section+"/detectors");

    m_DetectorThreads.resize(n);
    m_Detectors.resize(n);

    for (int i=0; i<n; n++) {
      settings->setArrayIndex(i);

      int detType = settings->value("detectorType", 0).toInt();

      QxrdDetectorThreadPtr detThread =
          QxrdDetectorThreadPtr(new QxrdDetectorThread(m_Saver, experiment(), sharedFromThis(), detType));

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
  QxrdDetectorThreadPtr detThread =
      QxrdDetectorThreadPtr(new QxrdDetectorThread(m_Saver, experiment(), sharedFromThis(), detType));

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

void QxrdMultipleAcquisition::clearDetectors()
{
  m_Detectors.resize(0);

  set_DetectorCount(0);
}

QxrdDetectorPtr QxrdMultipleAcquisition::detector(int n)
{
  return m_Detectors.value(n);
}

void QxrdMultipleAcquisition::onExposureTimeChanged()
{
}

void QxrdMultipleAcquisition::onBinningModeChanged()
{
}

void QxrdMultipleAcquisition::onCameraGainChanged()
{
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
}

void QxrdMultipleAcquisition::endAcquisition()
{
}

void QxrdMultipleAcquisition::shutdownAcquisition()
{
}
