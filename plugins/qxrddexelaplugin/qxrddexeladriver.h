#ifndef QXRDDEXELADRIVER_H
#define QXRDDEXELADRIVER_H

#include "qxrddetectordriver.h"
#include "qxrddexelasettings-ptr.h"
#include <QTimer>
#include <QMutex>
#include "BusScanner.h"

class QxrdDexelaDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDexelaDriver(QString name,
                   QxrdDexelaSettingsWPtr det,
                   QxrdExperimentWPtr expt,
                   QxrdAcqCommonWPtr acq);
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
  int scanForDetectors();

private:
  QxrdDexelaSettingsWPtr m_Dexela;
  QTimer                 m_Timer;

  static QMutex           m_Mutex;
  static BusScanner      *m_BusScanner;
  static int              m_DetectorCount;
  static int              m_Initialized;
  static QVector<DevInfo> m_Devices;
};

#endif // QXRDDEXELADETECTORDRIVER_H
