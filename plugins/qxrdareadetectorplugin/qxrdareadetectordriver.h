#ifndef QXRDAREADETECTORDRIVER_H
#define QXRDAREADETECTORDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrdareadetectorsettings-ptr.h"
#include <QTimer>
#include "qxrdareadetectorplugin-ptr.h"
#include "qcepimagedata-ptr.h"

class QxrdAreaDetectorDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdAreaDetectorDriver(QString name,
                         QxrdAreaDetectorPluginWPtr plugin,
                         QxrdAreaDetectorSettingsWPtr det,
                         QxrdExperimentWPtr expt,
                         QxrdAcqCommonWPtr acq);
  virtual ~QxrdAreaDetectorDriver();

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
  QxrdAreaDetectorPluginWPtr   m_Plugin;
  QxrdAreaDetectorSettingsWPtr m_AreaDetector;
  QTimer                       m_Timer;
  QcepUInt32ImageDataPtr       m_AccumulatedData;
};

Q_DECLARE_METATYPE(QxrdAreaDetectorDriver*)

#endif // QXRDAREADETECTORDRIVER_H
