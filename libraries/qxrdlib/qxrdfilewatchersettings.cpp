#include "qxrdfilewatchersettings.h"
#include <stdio.h>

QxrdFileWatcherSettings::QxrdFileWatcherSettings(QString name) :
  QxrdDetectorSettings(name, FileWatcherDetector)
{
}

void QxrdFileWatcherSettings::pushDefaultsToProxy(QxrdDetectorProxyPtr /*proxy*/)
{
//  QxrdDetectorSettings::pushDefaultsToProxy(proxy, FileWatcherDetector);

  printf("Need to implement QxrdFileWatcherSettings::pushDefaultsToProxy\n");
}

void QxrdFileWatcherSettings::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pushPropertiesToProxy(proxy);

  printf("Need to implement QxrdFileWatcherSettings::pushPropertiesToProxy\n");
}

void QxrdFileWatcherSettings::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pullPropertiesfromProxy(proxy);

  printf("Need to implement QxrdFileWatcherSettings::pullPropertiesfromProxy\n");
}

