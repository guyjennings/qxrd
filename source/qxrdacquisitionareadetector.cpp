#include "qxrdacquisitionareadetector.h"

QxrdAcquisitionAreaDetector::QxrdAcquisitionAreaDetector(QxrdSettingsSaver *saver,
                                                         QxrdExperiment *doc,
                                                         QxrdDataProcessor *proc,
                                                         QxrdAllocator *allocator)
  : QxrdAcquisition(saver, doc, proc, allocator)
{
}

void QxrdAcquisitionAreaDetector::initialize()
{
  QxrdAcquisition::initialize();
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
