#ifndef QXRDACQUISITIONAREADETECTOR_H
#define QXRDACQUISITIONAREADETECTOR_H

#include "qcepmacros.h"

#include "qxrdacquisition.h"

class QxrdAcquisitionThread;

class QxrdAcquisitionAreaDetector : public QxrdAcquisition
{
  Q_OBJECT;
public:
  QxrdAcquisitionAreaDetector(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator);

private slots:
  void initialize();

  void onExposureTimeChanged(double newTime);
  void onBinningModeChanged(int newMode);
  void onCameraGainChanged(int newGain);

private:
  void beginAcquisition();
  void setupExposureMenu(QDoubleSpinBox* cb);
  void setupCameraGainMenu(QComboBox* cb);
  void setupCameraBinningModeMenu(QComboBox *cb);
};

#endif // QXRDACQUISITIONAREADETECTOR_H
