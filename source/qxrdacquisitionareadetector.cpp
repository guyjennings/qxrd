#include "qxrdacquisitionareadetector.h"

QxrdAcquisitionAreaDetector::QxrdAcquisitionAreaDetector(QxrdSettingsSaverWPtr saver,
                                                         QxrdExperimentWPtr doc,
                                                         QxrdDataProcessorWPtr proc,
                                                         QxrdAllocatorWPtr allocator)
  : QxrdAcquisition(saver, doc, proc, allocator)
{
}

void QxrdAcquisitionAreaDetector::beginAcquisition()
{
}

void QxrdAcquisitionAreaDetector::endAcquisition()
{
}

void QxrdAcquisitionAreaDetector::shutdownAcquisition()
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
