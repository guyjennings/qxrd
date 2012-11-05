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
  virtual ~QxrdDetectorSimulated();

  void initialize();
  int detectorType() const;

public slots:
  void onExposureTimeChanged();

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
