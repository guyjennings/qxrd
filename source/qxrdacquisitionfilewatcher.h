#ifndef QXRDACQUISITIONFILEWATCHER_H
#define QXRDACQUISITIONFILEWATCHER_H

#include "qxrdacquisition.h"
#include "qxrddataprocessor.h"

class QxrdAcquisitionFileWatcher : public QxrdAcquisition
{
    Q_OBJECT
public:
    explicit QxrdAcquisitionFileWatcher(QxrdSettingsSaverPtr saver,
                                        QxrdExperimentWPtr doc,
                                        QxrdDataProcessorPtr proc,
                                        QxrdAllocatorPtr allocator,
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

  void setupExposureMenu(QDoubleSpinBox* cb);
  void setupCameraGainMenu(QComboBox* cb);
  void setupCameraBinningModeMenu(QComboBox *cb);

  QxrdAcquireDialogBase* controlPanel(QxrdWindow *win);
};

#endif // QXRDACQUISITIONFILEWATCHER_H
