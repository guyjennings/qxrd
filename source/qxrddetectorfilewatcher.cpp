#include "qxrddetectorfilewatcher.h"
#include "qxrddetectorthread.h"

QxrdDetectorFileWatcher::QxrdDetectorFileWatcher(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QxrdDetector(expt, acq)
{
}

int QxrdDetectorFileWatcher::detectorType() const
{
  return QxrdDetectorThread::FileWatcherDetector;
}
