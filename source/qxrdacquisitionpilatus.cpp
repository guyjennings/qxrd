#include "qxrdacquisitionpilatus.h"

QxrdAcquisitionPilatus::QxrdAcquisitionPilatus(QxrdDataProcessor *proc, QxrdAllocator *allocator)
  : QxrdAcquisition(proc, allocator)
{
}

void QxrdAcquisitionPilatus::initialize()
{
  QxrdAcquisition::initialize();
}

void QxrdAcquisitionPilatus::beginAcquisition()
{
}

void QxrdAcquisitionPilatus::onExposureTimeChanged(double /*newTime*/)
{
}

void QxrdAcquisitionPilatus::onBinningModeChanged(int /*newMode*/)
{
}

void QxrdAcquisitionPilatus::onCameraGainChanged(int /*newGain*/)
{
}

void QxrdAcquisitionPilatus::setupExposureMenu(QDoubleSpinBox * /*cb*/)
{
}

void QxrdAcquisitionPilatus::setupCameraGainMenu(QComboBox * /*cb*/)
{
}

void QxrdAcquisitionPilatus::setupCameraBinningModeMenu(QComboBox * /*cb*/)
{
}
