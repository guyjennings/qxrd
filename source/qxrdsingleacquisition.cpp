#include "qxrdsingleacquisition.h"
#include "qxrddetector.h"

QxrdSingleAcquisition::QxrdSingleAcquisition(QcepSettingsSaverWPtr saver,
                                             QxrdExperimentWPtr    doc,
                                             QxrdDataProcessorWPtr proc,
                                             QcepAllocatorWPtr     allocator)
  : QxrdAcquisition(saver, doc, proc, allocator),
    m_Detector()
{

}

void QxrdSingleAcquisition::setDetector(QxrdDetectorWPtr det)
{
  m_Detector = det;
}

QxrdDetectorPtr QxrdSingleAcquisition::detector()
{
  return m_Detector;
}

void QxrdSingleAcquisition::onExposureTimeChanged()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->onExposureTimeChanged();
  }
}

void QxrdSingleAcquisition::onBinningModeChanged()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->onBinningModeChanged();
  }
}

void QxrdSingleAcquisition::onCameraGainChanged()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->onCameraGainChanged();
  }
}


void QxrdSingleAcquisition::setupExposureMenu(QDoubleSpinBox *cb)
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->setupExposureMenu(cb, get_ExposureTime());
  }
}

void QxrdSingleAcquisition::setupCameraGainMenu(QComboBox *cb)
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->setupCameraGainMenu(cb, get_CameraGain());
  }
}

void QxrdSingleAcquisition::setupCameraBinningModeMenu(QComboBox *cb)
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->setupCameraBinningModeMenu(cb, get_BinningMode());
  }
}


void QxrdSingleAcquisition::beginAcquisition()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->beginAcquisition();
  }
}

void QxrdSingleAcquisition::endAcquisition()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->endAcquisition();
  }
}

void QxrdSingleAcquisition::shutdownAcquisition()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->shutdownAcquisition();
  }
}

