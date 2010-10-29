#ifndef QXRDACQUISITIONSIMULATED_H
#define QXRDACQUISITIONSIMULATED_H

#include "qcepmacros.h"

#include "qxrdacquisition.h"
#include <QTimer>

class QxrdAcquisitionSimulated : public QxrdAcquisition
{
  Q_OBJECT;
public:
  QxrdAcquisitionSimulated(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator);

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

private:
  void initialize();

  QTimer m_Timer;
};

#endif // QXRDACQUISITIONSIMULATED_H
