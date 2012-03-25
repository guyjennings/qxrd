#include "qxrddetector.h"

QxrdDetector::QxrdDetector(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QObject(),
  m_Experiment(expt),
  m_Acquisition(acq)
{
}

QxrdDetector::~QxrdDetector()
{
}

void QxrdDetector::initialize()
{
}

void QxrdDetector::onExposureTimeChanged(double newTime)
{
}

void QxrdDetector::onBinningModeChanged(int newMode)
{
}

void QxrdDetector::onCameraGainChanged(int newGain)
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
