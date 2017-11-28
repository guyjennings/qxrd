#ifndef QXRDPILATUSDRIVER_H
#define QXRDPILATUSDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrddetectorsettingspilatus-ptr.h"
#include <QTimer>

class QxrdPilatusDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdPilatusDriver(QString name,
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
  QxrdDetectorSettingsPilatusWPtr m_Pilatus;
  QTimer m_Timer;
};

#endif // QXRDPILATUSDRIVER_H
