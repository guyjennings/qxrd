#ifndef QXRDDEXELADETECTORDRIVER_H
#define QXRDDEXELADETECTORDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrddetectorsettingsdexela-ptr.h"
#include <QTimer>

class QxrdDexelaDetectorDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDexelaDetectorDriver(QString name,
                           QxrdDetectorSettingsPtr det,
                           QxrdExperimentPtr expt,
                           QxrdAcquisitionPtr acq);

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
  QxrdDetectorSettingsDexelaWPtr m_Dexela;
  QTimer m_Timer;
};

#endif // QXRDDEXELADETECTORDRIVER_H
