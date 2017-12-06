#include "qxrddetectordriverfilewatcher.h"
#include "qxrddetectorsettingsfilewatcher.h"

QxrdDetectorDriverFileWatcher::QxrdDetectorDriverFileWatcher(QString name,
    QxrdDetectorSettingsFileWatcherWPtr watcher,
    QxrdExperimentWPtr expt,
    QxrdAcquisitionWPtr acq) :
  QxrdDetectorDriver(name, watcher, expt, acq),
  m_FileWatcher(watcher)
{
#ifndef QT_NO_DEBUG
  printf("File Watcher Driver \"%s\" Constructed (Non-Implemented)\n", qPrintable(name));
#endif
}

QxrdDetectorDriverFileWatcher::~QxrdDetectorDriverFileWatcher()
{
#ifndef QT_NO_DEBUG
  printf("File Watcher Driver \"%s\" Destroyed\n", qPrintable(get_Name()));
#endif
}

bool QxrdDetectorDriverFileWatcher::startDetectorDriver()
{
  return false;
}

bool QxrdDetectorDriverFileWatcher::stopDetectorDriver()
{
  return false;
}

bool QxrdDetectorDriverFileWatcher::changeExposureTime(double /*exposure*/)
{
  return false;
}

bool QxrdDetectorDriverFileWatcher::beginAcquisition(double /*exposure*/)
{
  return false;
}

bool QxrdDetectorDriverFileWatcher::endAcquisition()
{
  return false;
}

bool QxrdDetectorDriverFileWatcher::shutdownAcquisition()
{
  return false;
}

void QxrdDetectorDriverFileWatcher::beginFrame()
{
}
