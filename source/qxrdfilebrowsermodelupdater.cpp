#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdapplication.h"
#include <QThread>

QxrdFileBrowserModelUpdater::QxrdFileBrowserModelUpdater(QxrdFileBrowserModel *browser, QObject *parent) :
  QObject(parent),
  m_BrowserModel(browser),
  m_RootPath(""),
  m_FileSystemWatcher(NULL),
  m_UpdateNeeded(0),
  m_UpdateTimer(),
  m_UpdateInterval(1000)
{
  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Browser Model Updater");
  }

  m_FileSystemWatcher = new QFileSystemWatcher(this);

  connect(m_BrowserModel,      SIGNAL(rootChanged(const QString&)),      this, SLOT(changeRoot(const QString&)));
  connect(m_FileSystemWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(changeContents(const QString&)), Qt::DirectConnection);
  connect(&m_UpdateTimer,      SIGNAL(timeout()),                        this, SLOT(updateTimeout()));

  m_UpdateTimer.setSingleShot(true);
  m_UpdateTimer.start(m_UpdateInterval);
}

void QxrdFileBrowserModelUpdater::shutdown()
{
  thread()->exit();
}

void QxrdFileBrowserModelUpdater::changeRoot(const QString& path)
{
  if (qcepDebug(DEBUG_BROWSER)) {
    g_Application->printMessage(tr("QxrdFileBrowserModelUpdater::changeRoot %1").arg(path));
  }

  QStringList dirs = m_FileSystemWatcher->directories();

  if (dirs.count() >= 1) {
    m_FileSystemWatcher->removePaths(dirs);
  }

  m_FileSystemWatcher->addPath(path);
  m_RootPath = path;
}

void QxrdFileBrowserModelUpdater::changeContents(const QString& path)
{
  if (qcepDebug(DEBUG_BROWSER)) {
    g_Application->printMessage(tr("QxrdFileBrowserModelUpdater::changeContents %1").arg(path));
  }

  m_UpdateNeeded.fetchAndStoreOrdered(1);
}

void QxrdFileBrowserModelUpdater::updateTimeout()
{
  if (m_UpdateNeeded.fetchAndStoreOrdered(0)) {
    g_Application->printMessage(tr("QxrdFileBrowserModelUpdater::updateTimeout update needed %1").arg(m_RootPath));
  }

  m_UpdateTimer.start(m_UpdateInterval);
}
