#include "qxrdacquisitionpilatus.h"

QxrdAcquisitionPilatus::QxrdAcquisitionPilatus(QxrdSettingsSaverWPtr saver,
                                               QxrdExperimentWPtr doc,
                                               QxrdDataProcessorWPtr proc,
                                               QxrdAllocatorWPtr allocator)
  : QxrdAcquisition(PilatusDetector, saver, doc, proc, allocator)
{
}

void QxrdAcquisitionPilatus::initialize()
{
  QxrdAcquisition::initialize();
}

void QxrdAcquisitionPilatus::beginAcquisition()
{
}

void QxrdAcquisitionPilatus::endAcquisition()
{
}

void QxrdAcquisitionPilatus::shutdownAcquisition()
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
