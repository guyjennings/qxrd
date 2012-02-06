#include "qxrdacquisitionpilatus.h"

QxrdAcquisitionPilatus::QxrdAcquisitionPilatus(QxrdSettingsSaverPtr saver,
                                               QxrdExperimentWPtr doc,
                                               QxrdDataProcessorWPtr proc,
                                               QxrdAllocatorPtr allocator,
                                               QSettings *settings,
                                               QString section)
  : QxrdAcquisition(PilatusDetector, saver, doc, proc, allocator)
{
  readSettings(settings, section);
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
