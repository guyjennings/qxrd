#ifndef QXRDFILEWATCHERDRIVER_H
#define QXRDFILEWATCHERDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrdfilewatchersettings-ptr.h"
#include <QTimer>
#include "qxrdfilewatcherplugin-ptr.h"

class QxrdFileWatcherDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdFileWatcherDriver(QString name,
                        QxrdFileWatcherPluginWPtr plugin,
                        QxrdDetectorSettingsWPtr det,
                        QxrdExperimentWPtr expt,
                        QxrdAcqCommonWPtr acq);
  virtual ~QxrdFileWatcherDriver();

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
  QxrdFileWatcherSettingsWPtr m_FileWatcher;
  QTimer m_Timer;
};

#endif // QXRDFILEWATCHERDRIVER_H
