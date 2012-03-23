#ifndef QXRDACQUISITIONFILEWATCHER_H
#define QXRDACQUISITIONFILEWATCHER_H

#include "qxrdacquisition.h"

class QxrdAcquisitionFileWatcher : public QxrdAcquisition
{
    Q_OBJECT
public:
    explicit QxrdAcquisitionFileWatcher(QxrdSettingsSaverPtr saver,
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

  QxrdAcquireDialogBase* controlPanel(QxrdWindow *win);
};

#endif // QXRDACQUISITIONFILEWATCHER_H
