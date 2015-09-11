#include "qxrddetectorfilewatcher.h"
#include "qxrddetectorthread.h"

QxrdDetectorFileWatcher::QxrdDetectorFileWatcher(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, QcepObject *parent) :
  QxrdDetector(saver, expt, acq, QxrdDetectorThread::FileWatcherDetector, parent)
{
}

void QxrdDetectorFileWatcher::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetector::pushDefaultsToProxy(proxy);

  printf("Need to implement QxrdDetectorFileWatcher::pushDefaultsToProxy\n");
}

void QxrdDetectorFileWatcher::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetector::pushPropertiesToProxy(proxy);

  printf("Need to implement QxrdDetectorFileWatcher::pushPropertiesToProxy\n");
}

void QxrdDetectorFileWatcher::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetector::pullPropertiesfromProxy(proxy);

  printf("Need to implement QxrdDetectorFileWatcher::pullPropertiesfromProxy\n");
}

