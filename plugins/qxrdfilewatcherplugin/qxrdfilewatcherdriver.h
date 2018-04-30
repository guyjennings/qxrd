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
                        QxrdFileWatcherSettingsWPtr det,
                        QxrdExperimentWPtr expt,
                        QxrdAcqCommonWPtr acq);
  virtual ~QxrdFileWatcherDriver();

public slots:
  void haltDetectorDriver();
  void startDetectorDriver();
  void stopDetectorDriver();
  void changeExposureTime(double exposure);
  void beginAcquisition(double exposure);
  void endAcquisition();
  void shutdownAcquisition();
  void beginFrame();

private slots:
  void onTimerTimeout();

private:
  QxrdFileWatcherSettingsWPtr m_FileWatcher;
  QTimer                      m_Timer;
};

Q_DECLARE_METATYPE(QxrdFileWatcherDriver*)

#endif // QXRDFILEWATCHERDRIVER_H
