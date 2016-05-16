#include "qxrddetectorfilewatcher.h"
#include "qxrddetectorthread.h"
#include <stdio.h>

QxrdDetectorFileWatcher::QxrdDetectorFileWatcher(QxrdExperimentWPtr    expt,
                                                 QxrdAcquisitionWPtr   acq,
                                                 int                   detNum) :
  QxrdDetector(expt, acq, QxrdDetectorThread::FileWatcherDetector, detNum)
{
}

void QxrdDetectorFileWatcher::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetector::pushDefaultsToProxy(proxy, QxrdDetectorThread::FileWatcherDetector);

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

