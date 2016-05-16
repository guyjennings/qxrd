#ifndef QXRDDETECTORSIMULATED_H
#define QXRDDETECTORSIMULATED_H

#include "qcepmacros.h"
#include "qxrddetector.h"
#include <QTimer>

class QxrdDetectorSimulated : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorSimulated(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum);
  virtual ~QxrdDetectorSimulated();

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);
  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

public slots:
  void startDetector();
  void stopDetector();

  virtual void onExposureTimeChanged();
  void onTimerTimeout();

public:
//  void setupExposureMenu(QDoubleSpinBox *cb, double initialExposure);

signals:

public:
  void beginAcquisition(double exposure);
  void endAcquisition();
  void shutdownAcquisition();

signals:

public slots:

private:
  QTimer m_Timer;
};

#endif // QXRDDETECTORSIMULATED_H
