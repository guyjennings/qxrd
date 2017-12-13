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
