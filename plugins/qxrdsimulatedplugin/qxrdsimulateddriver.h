#ifndef QXRDSIMULATEDDRIVER_H
#define QXRDSIMULATEDDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrdsimulatedsettings-ptr.h"
#include "qxrdsimulatedplugin-ptr.h"
#include <QTimer>
#include "qcepimagedata-ptr.h"

class QxrdSimulatedDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdSimulatedDriver(QString name,
                      QxrdSimulatedPluginWPtr plugin,
                      QxrdSimulatedSettingsWPtr det,
                      QxrdExperimentWPtr expt,
                      QxrdAcqCommonWPtr acq);
  virtual ~QxrdSimulatedDriver();

public slots:
  void haltDetectorDriver();
  void startDetectorDriver();
  void stopDetectorDriver();
  void changeExposureTime(double exposure);
  void beginAcquisition(double exposure);
  void endAcquisition();
  void shutdownAcquisition();
  void beginFrame();

private slots:
  void onTimerTimeout();

private:
  QxrdSimulatedPluginWPtr   m_Plugin;
  QxrdSimulatedSettingsWPtr m_Simulated;
  QTimer                    m_Timer;
  QcepUInt32ImageDataPtr    m_AccumulatedData;
};

Q_DECLARE_METATYPE(QxrdSimulatedDriver*)

#endif // QXRDSIMULATEDDRIVER_H
