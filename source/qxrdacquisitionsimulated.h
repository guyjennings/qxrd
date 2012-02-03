#ifndef QXRDACQUISITIONSIMULATED_H
#define QXRDACQUISITIONSIMULATED_H

#include "qxrdacquisition.h"
#include "qxrddataprocessor.h"
#include <QTimer>

class QxrdAcquisitionSimulated : public QxrdAcquisition
{
  Q_OBJECT
public:
  QxrdAcquisitionSimulated(QxrdSettingsSaverPtr saver,
                           QxrdExperimentPtr doc,
                           QxrdDataProcessorPtr proc,
                           QxrdAllocatorPtr allocator,
                           QSettings *settings,
                           QString section);

public slots:
  void onExposureTimeChanged(double newTime);
  void onBinningModeChanged(int newMode);
  void onCameraGainChanged(int newGain);

  void onTimerTimeout();

public:
  void setupExposureMenu(QDoubleSpinBox *cb);
  void setupCameraGainMenu(QComboBox *cb);
  void setupCameraBinningModeMenu(QComboBox *cb);

signals:

protected:
  void beginAcquisition();
  void endAcquisition();
  void shutdownAcquisition();

private:
  void initialize();

  QTimer m_Timer;
};

#endif // QXRDACQUISITIONSIMULATED_H
