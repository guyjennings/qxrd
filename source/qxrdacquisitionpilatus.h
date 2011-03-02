#ifndef QXRDACQUISITIONPILATUS_H
#define QXRDACQUISITIONPILATUS_H

#include "qxrdacquisition.h"

class QxrdAcquisitionPilatus : public QxrdAcquisition
{
  Q_OBJECT;
public:
  QxrdAcquisitionPilatus(QxrdDataProcessor *proc, QxrdAllocator *allocator);

private slots:
  void initialize();

  void onExposureTimeChanged(double newTime);
  void onBinningModeChanged(int newMode);
  void onCameraGainChanged(int newGain);

private:
  void beginAcquisition();
  void endAcquisition();

  void setupExposureMenu(QDoubleSpinBox *cb);
  void setupCameraGainMenu(QComboBox *cb);
  void setupCameraBinningModeMenu(QComboBox *cb);
};

#endif // QXRDACQUISITIONPILATUS_H
