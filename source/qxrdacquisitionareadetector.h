#ifndef QXRDACQUISITIONAREADETECTOR_H
#define QXRDACQUISITIONAREADETECTOR_H

#include "qxrdacquisition.h"

class QxrdAcquisitionAreaDetector : public QxrdAcquisition
{
  Q_OBJECT
public:
  QxrdAcquisitionAreaDetector(QxrdExperiment *doc,
                              QxrdDataProcessor *proc,
                              QxrdAllocator *allocator);

private slots:
  void initialize();

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
