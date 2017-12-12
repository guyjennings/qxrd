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
                      QxrdAcquisitionWPtr acq);

public slots:
  virtual bool startDetectorDriver();
  virtual bool stopDetectorDriver();
  virtual bool changeExposureTime(double exposure);
  virtual bool beginAcquisition(double exposure);
  virtual bool endAcquisition();
  virtual bool shutdownAcquisition();
  virtual void beginFrame();

private slots:
  void onTimerTimeout();

private:
  QxrdSimulatedSettingsWPtr m_Simulated;
  QTimer m_Timer;
};

#endif // QXRDSIMULATEDDRIVER_H
