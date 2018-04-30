#ifndef QXRDALLIEDVISIONDRIVER_H
#define QXRDALLIEDVISIONDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrdalliedvisionsettings-ptr.h"
#include "qxrdalliedvisionplugin-ptr.h"
#include <QTimer>
#include "qcepimagedata-ptr.h"

class QxrdAlliedVisionDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdAlliedVisionDriver(QString name,
                      QxrdAlliedVisionPluginWPtr plugin,
                      QxrdAlliedVisionSettingsWPtr det,
                      QxrdExperimentWPtr expt,
                      QxrdAcqCommonWPtr acq);
  virtual ~QxrdAlliedVisionDriver();

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
  QxrdAlliedVisionPluginWPtr   m_Plugin;
  QxrdAlliedVisionSettingsWPtr m_AlliedVision;
  QTimer                       m_Timer;
  QcepUInt32ImageDataPtr       m_AccumulatedData;
};

Q_DECLARE_METATYPE(QxrdAlliedVisionDriver*)

#endif // QXRDALLIEDVISIONDRIVER_H
