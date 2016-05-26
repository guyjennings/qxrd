#ifndef QXRDDETECTORDRIVERFILEWATCHER_H
#define QXRDDETECTORDRIVERFILEWATCHER_H

#include "qxrddetectordriver.h"
#include "qxrddetectorsettingsfilewatcher-ptr.h"

class QxrdDetectorDriverFileWatcher : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverFileWatcher(QString name,
                                QxrdDetectorSettingsFileWatcherPtr watcher,
                                QxrdExperimentPtr expt,
                                QxrdAcquisitionPtr acq);

private:
  QxrdDetectorSettingsFileWatcherWPtr m_FileWatcher;
};

#endif // QXRDDETECTORDRIVERFILEWATCHER_H
