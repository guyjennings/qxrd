#ifndef QXRDACQUISITIONPILATUS_H
#define QXRDACQUISITIONPILATUS_H

#include "qcepmacros.h"

#include "qxrdacquisition.h"

class QxrdAcquisitionThread;

class QxrdAcquisitionPilatus : public QxrdAcquisition
{
  Q_OBJECT;
public:
  QxrdAcquisitionPilatus(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator);

private slots:
  void initialize();

  void onExposureTimeChanged(double newTime);
  void onBinningModeChanged(int newMode);
  void onCameraGainChanged(int newGain);

private:
  void beginAcquisition();
  void setupExposureMenu(QDoubleSpinBox *cb);
  void setupCameraGainMenu(QComboBox *cb);
  void setupCameraBinningModeMenu(QComboBox *cb);
};

#endif // QXRDACQUISITIONPILATUS_H
