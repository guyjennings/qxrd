#include "qxrddetectorfilewatcher.h"
#include "qxrddetectorthread.h"

QxrdDetectorFileWatcher::QxrdDetectorFileWatcher(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, QcepObject *parent) :
  QxrdDetector(saver, expt, acq, QxrdDetectorThread::FileWatcherDetector, parent)
{
}
