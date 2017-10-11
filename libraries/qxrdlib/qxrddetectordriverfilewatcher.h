#ifndef QXRDDETECTORDRIVERFILEWATCHER_H
#define QXRDDETECTORDRIVERFILEWATCHER_H

#include "qxrdlib_global.h"
#include "qxrddetectordriver.h"
#include "qxrddetectorsettingsfilewatcher-ptr.h"

class QXRD_EXPORT QxrdDetectorDriverFileWatcher : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverFileWatcher(QString name,
                                QxrdDetectorSettingsFileWatcherPtr watcher,
                                QxrdExperimentPtr expt,
                                QxrdAcquisitionPtr acq);
  virtual ~QxrdDetectorDriverFileWatcher();

public slots:
  virtual bool startDetectorDriver();
  virtual bool stopDetectorDriver();
  virtual bool changeExposureTime(double exposure);
  virtual bool beginAcquisition(double exposure);
  virtual bool endAcquisition();
  virtual bool shutdownAcquisition();
  virtual void beginFrame();

private:
  QxrdDetectorSettingsFileWatcherWPtr m_FileWatcher;
};

#endif // QXRDDETECTORDRIVERFILEWATCHER_H