#ifndef QXRDDEXELADRIVER_H
#define QXRDDEXELADRIVER_H

#include "qxrddetectordriver.h"
#include "qxrddexelasettings-ptr.h"
#include <QTimer>

class QxrdDexelaDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDexelaDriver(QString name,
                           QxrdDexelaSettingsWPtr det,
                           QxrdExperimentWPtr expt,
                           QxrdAcquisitionWPtr acq);
  virtual ~QxrdDexelaDriver();

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
  QxrdDexelaSettingsWPtr m_Dexela;
  QTimer m_Timer;
};

#endif // QXRDDEXELADETECTORDRIVER_H
