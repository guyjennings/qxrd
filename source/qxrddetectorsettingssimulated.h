#ifndef QXRDDETECTORSIMULATED_H
#define QXRDDETECTORSIMULATED_H

#include "qcepmacros.h"
#include "qxrddetectorsettings.h"
#include <QTimer>

class QxrdDetectorSettingsSimulated : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  explicit QxrdDetectorSettingsSimulated(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum);
  virtual ~QxrdDetectorSettingsSimulated();

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
