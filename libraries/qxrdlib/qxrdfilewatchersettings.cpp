#include "qxrdfilewatchersettings.h"
#include "qxrdfilewatchersettings-ptr.h"
#include <stdio.h>
#include "qxrdfilewatcherdialog.h"
#include "qxrdfilewatcherdialog-ptr.h"
#include <QThread>

QxrdFileWatcherSettings::QxrdFileWatcherSettings(QString name) :
  QxrdDetectorSettings(name, FileWatcher)
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

void QxrdFileWatcherSettings::configureDetector()
{
  GUI_THREAD_CHECK;

  QxrdFileWatcherSettingsPtr myself =
      qSharedPointerDynamicCast<QxrdFileWatcherSettings>(sharedFromThis());

  QxrdFileWatcherDialogPtr dlg =
      QxrdFileWatcherDialogPtr(
        new QxrdFileWatcherDialog(myself));

  if (dlg) {
    dlg->exec();
  }
}
