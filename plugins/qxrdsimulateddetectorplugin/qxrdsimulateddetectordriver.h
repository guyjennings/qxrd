#ifndef QXRDSIMULATEDDETECTORDRIVER_H
#define QXRDSIMULATEDDETECTORDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrddetectorsettingssimulated-ptr.h"
#include <QTimer>

class QxrdSimulatedDetectorDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdSimulatedDetectorDriver(QString name,
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
  QxrdDetectorSettingsSimulatedWPtr m_Simulated;
  QTimer m_Timer;
};

#endif // QXRDSIMULATEDDETECTORDRIVER_H
