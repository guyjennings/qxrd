#include "qxrddetector.h"
#include "qxrddetectorthread.h"

QxrdDetector::QxrdDetector(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QObject(),
  m_Experiment(expt),
  m_Acquisition(acq)
{
}

QxrdDetector::~QxrdDetector()
{
}

int  QxrdDetector::detectorType() const
{
  return QxrdDetectorThread::NoDetector;
}

QString QxrdDetector::detectorTypeName() const
{
  return QxrdDetectorThread::detectorTypeName(detectorType());
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

void QxrdDetector::setupExposureMenu(QDoubleSpinBox *cb)
{
}

void QxrdDetector::setupCameraGainMenu(QComboBox *cb)
{
}

void QxrdDetector::setupCameraBinningModeMenu(QComboBox *cb)
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
