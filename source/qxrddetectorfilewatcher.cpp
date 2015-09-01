#include "qxrddetectorfilewatcher.h"
#include "qxrddetectorthread.h"

QxrdDetectorFileWatcher::QxrdDetectorFileWatcher(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QxrdDetector(saver, expt, acq, QxrdDetectorThread::FileWatcherDetector)
{
}
