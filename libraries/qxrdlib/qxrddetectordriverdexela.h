#ifndef QXRDDETECTORDRIVERDEXELA_H
#define QXRDDETECTORDRIVERDEXELA_H

#include "qxrdlib_global.h"
#include "qxrddetectordriver.h"
#include "qxrddetectorsettingsdexela-ptr.h"
#include <QTimer>

class QXRD_EXPORT QxrdDetectorDriverDexela : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverDexela(QString name,
                              QxrdDetectorSettingsDexelaPtr sim,
                              QxrdExperimentPtr expt,
                              QxrdAcquisitionPtr acq);
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
  QxrdDetectorSettingsDexelaWPtr m_Dexela;
  QTimer m_Timer;
};

#endif // QXRDDETECTORDRIVERDEXELA_H
