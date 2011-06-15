#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdapplication.h"
#include <QThread>

QxrdFileBrowserModelUpdater::QxrdFileBrowserModelUpdater(QxrdFileBrowserModel *browser, QObject *parent) :
    QObject(parent)
{
  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Browser Model Updater");
  }
}

void QxrdFileBrowserModelUpdater::shutdown()
{
  thread()->exit();
}

