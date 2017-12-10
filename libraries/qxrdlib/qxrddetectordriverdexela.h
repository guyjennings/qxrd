#ifndef QXRDDETECTORDRIVERDEXELA_H
#define QXRDDETECTORDRIVERDEXELA_H

#include "qxrdlib_global.h"
#include "qxrddetectordriver.h"
#include "qxrddexelasettings-ptr.h"
#include <QTimer>

//TODO: move to plugin
class QXRD_EXPORT QxrdDetectorDriverDexela : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverDexela(QString name,
                              QxrdDexelaSettingsWPtr sim,
                              QxrdExperimentWPtr expt,
                              QxrdAcquisitionWPtr acq);
  virtual ~QxrdDetectorDriverDexela();

public slots:
  bool startDetectorDriver();
  bool stopDetectorDriver();
  bool changeExposureTime(double expos);
  bool beginAcquisition(double exposure);
  bool endAcquisition();
  bool shutdownAcquisition();
  void beginFrame();

private slots:
  void onTimerTimeout();

private:
  QxrdDexelaSettingsWPtr m_Dexela;
  QTimer m_Timer;
};

#endif // QXRDDETECTORDRIVERDEXELA_H
