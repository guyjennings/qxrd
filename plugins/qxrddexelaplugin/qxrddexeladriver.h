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
  void onAcquiredFrame       (int fc, int buf);

private:
  int scanForDetectors();

  void        callback       (int fc, int buf, DexelaDetector *det);
  static void staticCallback (int fc, int buf, DexelaDetector *det);

private:
  QxrdDexelaSettingsWPtr  m_Dexela;
  int                     m_DetectorIndex;
  DexelaDetector         *m_DexelaDetector;
  int                     m_XDim;
  int                     m_YDim;

private: // Static members...
  static QMutex           m_Mutex;
  static BusScanner      *m_BusScanner;
  static int              m_DetectorCount;
  static int              m_Initialized;
  static QVector<DevInfo> m_Devices;
};

#endif // QXRDDEXELADETECTORDRIVER_H
