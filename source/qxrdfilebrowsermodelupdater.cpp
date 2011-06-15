#include "qxrdfilebrowsermodelupdater.h"
#include <QThread>

QxrdFileBrowserModelUpdater::QxrdFileBrowserModelUpdater(QxrdFileBrowserModel *browser, QObject *parent) :
    QObject(parent)
{
}

void QxrdFileBrowserModelUpdater::shutdown()
{
  thread()->exit();
}

