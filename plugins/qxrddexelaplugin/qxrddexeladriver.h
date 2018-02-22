#ifndef QXRDDEXELADRIVER_H
#define QXRDDEXELADRIVER_H

#include "qxrddetectordriver.h"
#include "qxrddexelasettings-ptr.h"
#include <QTimer>
#include <QMutex>
#include "BusScanner.h"
#include "qxrddexelaplugin-ptr.h"

class QxrdDexelaDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDexelaDriver(QString name,
                   QxrdDexelaPluginWPtr plugin,
                   QxrdDexelaSettingsWPtr det,
                   QxrdExperimentWPtr expt,
                   QxrdAcqCommonWPtr acq);
  virtual ~QxrdDexelaDriver();

public slots:
  virtual void startDetectorDriver();
  virtual void stopDetectorDriver();
  virtual void changeExposureTime(double exposure);
  virtual void beginAcquisition(double exposure);
  virtual void endAcquisition();
  virtual void shutdownAcquisition();
  virtual void beginFrame();

private slots:
  void onAcquiredFrame       (int fc, int buf);

private:
//  void scanForDetectors();

  static void staticCallback (int fc, int buf, DexelaDetector *det);

private:
  QxrdDexelaSettingsWPtr  m_Dexela;
  QxrdDexelaPluginWPtr    m_DexelaPlugin;
  DexelaDetector         *m_DexelaDetector;
  int                     m_XDim;
  int                     m_YDim;

private: // Static members...
  static QMutex           m_Mutex;
//  static BusScanner      *m_BusScanner;
//  static int              m_DetectorCount;
//  static int              m_Initialized;
//  static QVector<DevInfo> m_Devices;
};

#endif // QXRDDEXELADETECTORDRIVER_H
