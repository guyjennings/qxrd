#ifndef QXRDFILEWATCHERDRIVER_H
#define QXRDFILEWATCHERDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrddetectorsettingsfilewatcher-ptr.h"
#include <QTimer>

class QxrdFileWatcherDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdFileWatcherDriver(QString name,
                        QxrdDetectorSettingsWPtr det,
                        QxrdExperimentWPtr expt,
                        QxrdAcquisitionWPtr acq);

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
  QxrdDetectorSettingsFileWatcherWPtr m_FileWatcher;
  QTimer m_Timer;
};

#endif // QXRDFILEWATCHERDRIVER_H
