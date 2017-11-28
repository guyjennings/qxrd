#include "qxrddetectorsettingsfilewatcher.h"
#include <stdio.h>

QxrdDetectorSettingsFileWatcher::QxrdDetectorSettingsFileWatcher(QxrdApplicationWPtr app,
                                                                 QxrdExperimentWPtr    expt,
                                                                 QxrdAcquisitionWPtr   acq,
                                                                 int                   detNum) :
  QxrdDetectorSettings(app, expt, acq, FileWatcherDetector, detNum)
{
}

void QxrdDetectorSettingsFileWatcher::pushDefaultsToProxy(QxrdDetectorProxyPtr /*proxy*/)
{
//  QxrdDetectorSettings::pushDefaultsToProxy(proxy, FileWatcherDetector);

  printf("Need to implement QxrdDetectorSettingsFileWatcher::pushDefaultsToProxy\n");
}

void QxrdDetectorSettingsFileWatcher::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pushPropertiesToProxy(proxy);

  printf("Need to implement QxrdDetectorSettingsFileWatcher::pushPropertiesToProxy\n");
}

void QxrdDetectorSettingsFileWatcher::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pullPropertiesfromProxy(proxy);

  printf("Need to implement QxrdDetectorSettingsFileWatcher::pullPropertiesfromProxy\n");
}

