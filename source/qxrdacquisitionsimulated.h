#ifndef QXRDACQUISITIONSIMULATED_H
#define QXRDACQUISITIONSIMULATED_H

#include "qxrdacquisition.h"
#include <QTimer>

class QxrdAcquisitionSimulated : public QxrdAcquisition
{
  Q_OBJECT;
public:
  QxrdAcquisitionSimulated(QxrdDataProcessor *proc, QxrdAllocator *allocator);

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

private:
  void initialize();

  QTimer m_Timer;
};

#endif // QXRDACQUISITIONSIMULATED_H
