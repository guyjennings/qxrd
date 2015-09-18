#ifndef QXRDDETECTORSIMULATED_H
#define QXRDDETECTORSIMULATED_H

#include "qcepmacros.h"
#include "qxrddetector.h"
#include <QTimer>

class QxrdDetectorSimulated : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorSimulated(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum, QcepObject *parent);
  virtual ~QxrdDetectorSimulated();

  void initialize();

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);
  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

public slots:
  virtual void onExposureTimeChanged();
  void onTimerTimeout();

public:
  void setupExposureMenu(QDoubleSpinBox *cb, double initialExposure);

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
