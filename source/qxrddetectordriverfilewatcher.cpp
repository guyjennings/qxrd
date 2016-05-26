#include "qxrddetectordriverfilewatcher.h"
#include "qxrddetectorsettingsfilewatcher.h"

QxrdDetectorDriverFileWatcher::QxrdDetectorDriverFileWatcher(
    QString name,
    QxrdDetectorSettingsFileWatcherPtr watcher,
    QxrdExperimentPtr expt,
    QxrdAcquisitionPtr acq) :
  QxrdDetectorDriver(name, watcher, expt, acq),
  m_FileWatcher(watcher)
{
}
