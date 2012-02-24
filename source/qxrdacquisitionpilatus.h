#ifndef QXRDACQUISITIONPILATUS_H
#define QXRDACQUISITIONPILATUS_H

#include "qxrdacquisition.h"

class QxrdAcquisitionPilatus : public QxrdAcquisition
{
  Q_OBJECT
public:
  QxrdAcquisitionPilatus(QxrdSettingsSaverPtr saver,
                         QxrdExperimentWPtr doc,
                         QxrdDataProcessorWPtr proc,
                         QxrdAllocatorWPtr allocator,
                         QSettings *settings,
                         QString section);

private slots:
  void initialize();

  void onExposureTimeChanged(double newTime);
  void onBinningModeChanged(int newMode);
  void onCameraGainChanged(int newGain);

protected:
  void beginAcquisition();
  void endAcquisition();
  void shutdownAcquisition();

  void setupExposureMenu(QDoubleSpinBox *cb);
  void setupCameraGainMenu(QComboBox *cb);
  void setupCameraBinningModeMenu(QComboBox *cb);
};

#endif // QXRDACQUISITIONPILATUS_H

class QxrdAcquisitionPilatus;
