#ifndef QXRDAREADETECTORDRIVER_H
#define QXRDAREADETECTORDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrdareadetectorsettings-ptr.h"
#include <QTimer>

class QxrdAreaDetectorDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdAreaDetectorDriver(QString name,
                         QxrdDetectorSettingsWPtr det,
                         QxrdExperimentWPtr expt,
                         QxrdAcqCommonWPtr acq);
  virtual ~QxrdAreaDetectorDriver();

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
  QxrdAreaDetectorSettingsWPtr m_AreaDetector;
  QTimer m_Timer;
};

#endif // QXRDAREADETECTORDRIVER_H
