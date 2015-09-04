#ifndef QXRDDETECTORSIMULATED_H
#define QXRDDETECTORSIMULATED_H

#include "qcepmacros.h"
#include "qxrddetector.h"
#include <QTimer>

class QxrdDetectorSimulated : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorSimulated(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, QcepObject *parent);
  virtual ~QxrdDetectorSimulated();

  void initialize();

public slots:
  virtual void onExposureTimeChanged();
  virtual void onBinningModeChanged();
  virtual void onCameraGainChanged();

  void onTimerTimeout();

public:
  void setupExposureMenu(QDoubleSpinBox *cb, double initialExposure);
  void setupCameraGainMenu(QComboBox *cb, int initialGain);
  void setupCameraBinningModeMenu(QComboBox *cb, int initialBinning);

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
