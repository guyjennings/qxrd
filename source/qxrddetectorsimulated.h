#ifndef QXRDDETECTORSIMULATED_H
#define QXRDDETECTORSIMULATED_H

#include "qcepmacros.h"
#include "qxrddetector.h"
#include <QTimer>

class QxrdDetectorSimulated : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorSimulated(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);

  void initialize();
  int detectorType() const;

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

signals:

public slots:

private:
  QTimer m_Timer;
};

#endif // QXRDDETECTORSIMULATED_H
