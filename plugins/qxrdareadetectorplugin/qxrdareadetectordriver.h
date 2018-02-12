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
  QxrdAreaDetectorSettingsWPtr m_AreaDetector;
  QTimer m_Timer;
};

#endif // QXRDAREADETECTORDRIVER_H