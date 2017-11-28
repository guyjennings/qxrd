#ifndef QXRDDETECTORDRIVERSIMULATED_H
#define QXRDDETECTORDRIVERSIMULATED_H

#include "qxrdlib_global.h"
#include "qxrddetectordriver.h"
#include "qxrddetectorsettingssimulated-ptr.h"
#include <QTimer>

//TODO: move to plugin
class QXRD_EXPORT QxrdDetectorDriverSimulated : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverSimulated(QString name,
                              QxrdDetectorSettingsSimulatedPtr sim,
                              QxrdExperimentPtr expt,
                              QxrdAcquisitionPtr acq);
  virtual ~QxrdDetectorDriverSimulated();

public slots:
  bool startDetectorDriver();
  bool stopDetectorDriver();
  bool changeExposureTime(double expos);
  bool beginAcquisition(double exposure);
  bool endAcquisition();
  bool shutdownAcquisition();
  void beginFrame();

private slots:
  void onTimerTimeout();

private:
  QxrdDetectorSettingsSimulatedWPtr m_Simulated;
  QTimer m_Timer;
};

#endif // QXRDDETECTORDRIVERSIMULATED_H
