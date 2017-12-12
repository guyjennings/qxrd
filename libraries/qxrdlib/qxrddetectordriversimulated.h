#ifndef QXRDDETECTORDRIVERSIMULATED_H
#define QXRDDETECTORDRIVERSIMULATED_H

#include "qxrdlib_global.h"
#include "qxrddetectordriver.h"
#include "qxrdsimulatedsettings-ptr.h"
#include <QTimer>

//TODO: move to plugin
class QXRD_EXPORT QxrdDetectorDriverSimulated : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverSimulated(QString name,
                              QxrdSimulatedSettingsWPtr sim,
                              QxrdExperimentWPtr expt,
                              QxrdAcquisitionWPtr acq);
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
  QxrdSimulatedSettingsWPtr m_Simulated;
  QTimer m_Timer;
};

#endif // QXRDDETECTORDRIVERSIMULATED_H
