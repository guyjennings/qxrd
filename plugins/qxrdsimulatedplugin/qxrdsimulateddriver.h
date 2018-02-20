#ifndef QXRDSIMULATEDDRIVER_H
#define QXRDSIMULATEDDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrdsimulatedsettings-ptr.h"
#include <QTimer>

class QxrdSimulatedDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdSimulatedDriver(QString name,
                      QxrdDetectorSettingsWPtr det,
                      QxrdExperimentWPtr expt,
                      QxrdAcqCommonWPtr acq);
  virtual ~QxrdSimulatedDriver();

public slots:
  virtual void startDetectorDriver();
  virtual void stopDetectorDriver();
  virtual void changeExposureTime(double exposure);
  virtual void beginAcquisition(double exposure);
  virtual void endAcquisition();
  virtual void shutdownAcquisition();
  virtual void beginFrame();

private slots:
  void onTimerTimeout();

private:
  QxrdSimulatedSettingsWPtr m_Simulated;
  QTimer m_Timer;
};

#endif // QXRDSIMULATEDDRIVER_H
