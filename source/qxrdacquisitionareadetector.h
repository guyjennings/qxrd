#ifndef QXRDACQUISITIONAREADETECTOR_H
#define QXRDACQUISITIONAREADETECTOR_H

#include "qxrdacquisition.h"

class QxrdAcquisitionAreaDetector : public QxrdAcquisition
{
  Q_OBJECT
public:
  QxrdAcquisitionAreaDetector(QxrdSettingsSaverWPtr saver,
                              QxrdExperimentWPtr doc,
                              QxrdDataProcessorWPtr proc,
                              QxrdAllocatorWPtr allocator);

private slots:
  void onExposureTimeChanged(double newTime);
  void onBinningModeChanged(int newMode);
  void onCameraGainChanged(int newGain);

protected:
  void beginAcquisition();
  void endAcquisition();
  void shutdownAcquisition();

  void setupExposureMenu(QDoubleSpinBox* cb);
  void setupCameraGainMenu(QComboBox* cb);
  void setupCameraBinningModeMenu(QComboBox *cb);
};

#endif // QXRDACQUISITIONAREADETECTOR_H
