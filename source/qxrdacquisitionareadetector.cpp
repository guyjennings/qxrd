#include "qxrdacquisitionareadetector.h"

QxrdAcquisitionAreaDetector::QxrdAcquisitionAreaDetector(QxrdDataProcessor *proc, QxrdAllocator *allocator)
  : QxrdAcquisition(proc, allocator)
{
}

void QxrdAcquisitionAreaDetector::initialize()
{
  QxrdAcquisition::initialize();
}

void QxrdAcquisitionAreaDetector::beginAcquisition()
{
}

void QxrdAcquisitionAreaDetector::onExposureTimeChanged(double /*newTime*/)
{
}

void QxrdAcquisitionAreaDetector::onBinningModeChanged(int /*newMode*/)
{
}

void QxrdAcquisitionAreaDetector::onCameraGainChanged(int /*newGain*/)
{
}

void QxrdAcquisitionAreaDetector::setupExposureMenu(QDoubleSpinBox * /*cb*/)
{
}

void QxrdAcquisitionAreaDetector::setupCameraGainMenu(QComboBox * /*cb*/)
{
}

void QxrdAcquisitionAreaDetector::setupCameraBinningModeMenu(QComboBox * /*cb*/)
{
}
