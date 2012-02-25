#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdapplication.h"
#include <QThread>
#include <QDirIterator>
#include <QThreadStorage>
#include "qxrdfilebrowsermodel.h"

QxrdFileBrowserModelUpdater::QxrdFileBrowserModelUpdater(QxrdFileBrowserModel *browser, QObject *parent) :
  QObject(parent),
  m_BrowserModel(browser),
  m_RootPath(""),
  m_FileSystemWatcher(NULL),
  m_UpdateNeeded(1),
  m_UpdateTimer(),
  m_UpdateInterval(1000),
  m_PreviousUpdate(QDateTime::currentDateTime()),
  m_GenerateUpdates(true)
{
  if (g_Application && qcepDebug(DEBUG_BROWSER)) {
    g_Application->printMessage("Starting Browser Model Updater");
  }

  m_FileSystemWatcher = new QFileSystemWatcher(this);
  //  m_FileSystemWatcher->setObjectName(QLatin1String("_qt_autotest_force_engine_poller"));

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
  if (g_Application && qcepDebug(DEBUG_BROWSER)) {
    g_Application->printMessage(tr("QxrdFileBrowserModelUpdater::changeRoot %1").arg(path));
  }

  QStringList dirs = m_FileSystemWatcher->directories();

  if (dirs.count() >= 1) {
    m_FileSystemWatcher->removePaths(dirs);
  }

  m_FileSystemWatcher->addPath(path);
  m_RootPath = path;
}

void QxrdFileBrowserModelUpdater::needUpdate()
{
  m_UpdateNeeded.fetchAndStoreOrdered(1);
}

void QxrdFileBrowserModelUpdater::changeContents(const QString& path)
{
  if (g_Application && qcepDebug(DEBUG_BROWSER)) {
    g_Application->printMessage(tr("QxrdFileBrowserModelUpdater::changeContents %1").arg(path));
  }

  m_UpdateNeeded.fetchAndStoreOrdered(1);
}

void QxrdFileBrowserModelUpdater::updateTimeout()
{
  if (m_UpdateNeeded.fetchAndStoreOrdered(0)) {
    if (g_Application && qcepDebug(DEBUG_BROWSER)) {
      g_Application->printMessage(tr("QxrdFileBrowserModelUpdater::updateTimeout update needed %1").arg(m_RootPath));
    }

    updateContents();
  }

  m_UpdateTimer.start(m_UpdateInterval);
}

typedef QxrdFileBrowserModelUpdater *QxrdFileBrowserModelUpdaterPtr;

QThreadStorage<QxrdFileBrowserModelUpdaterPtr*> g_Updaters;

bool QxrdFileBrowserModelUpdater::updateNeeded()
{
  return m_UpdateNeeded.fetchAndAddOrdered(0);
}

void sortInterruptCheck()
{
  if (g_Updaters.hasLocalData()) {
    QxrdFileBrowserModelUpdaterPtr *updaterp = g_Updaters.localData();

    if (updaterp) {
      QxrdFileBrowserModelUpdater *updater = *updaterp;

      if (updater && updater->updateNeeded()) {
        throw 0;
      }
    }
  }
}

bool fileNameLessThan(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.fileName().toLower() < f2.fileName().toLower();
}

bool fileNameGreaterThan(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.fileName().toLower() > f2.fileName().toLower();
}

bool fileSizeLessThan(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.size() < f2.size();
}

bool fileSizeGreaterThan(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.size() > f2.size();
}

bool fileDateLessThan(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.lastModified() < f2.lastModified();
}

bool fileDateGreaterThan(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.lastModified() > f2.lastModified();
}

void QxrdFileBrowserModelUpdater::updateContents()
{
  QTime tic;
  tic.start();

  g_Updaters.setLocalData(new QxrdFileBrowserModelUpdaterPtr(this));

  m_UpdateNeeded.fetchAndStoreOrdered(0);

  QDirIterator iterd(m_RootPath);
  QDirIterator iter(m_RootPath, m_BrowserModel->nameFilters());
  QVector<QFileInfo> dirs;
  QVector<QFileInfo> files;

  while (iterd.hasNext()) {
    if (m_UpdateNeeded.fetchAndAddOrdered(0)) {
      if (g_Application && qcepDebug(DEBUG_BROWSER)) {
        g_Application->printMessage("Directory list generation abandoned");
      }

      return;
    }

    QString filePath = iterd.next();
    QFileInfo fileInfo(m_RootPath, filePath);

    if (fileInfo.isDir()) {
      QString dirName = fileInfo.fileName();

      if ((dirName != ".") && (dirName != "..")) {
        dirs.append(fileInfo);
      }
    }
  }

  while (iter.hasNext()) {
    if (m_UpdateNeeded.fetchAndAddOrdered(0)) {
      if (g_Application && qcepDebug(DEBUG_BROWSER)) {
        g_Application->printMessage("File list generation abandoned");
      }

      return;
    }

    QString filePath = iter.next();
    QFileInfo fileInfo(m_RootPath, filePath);

    if (fileInfo.isDir()) {
    } else {
      files.append(fileInfo);
    }
  }

  m_Directories  = dirs;
  m_Files        = files;

  if (g_Application && qcepDebug(DEBUG_BROWSER)) {
    g_Application->printMessage(tr("Update file browser took %1 msec").arg(tic.restart()));
    g_Application->printMessage(tr("File Path %1: %2 dirs, %3 files")
                      .arg(m_RootPath).arg(m_Directories.count()).arg(m_Files.count()));
  }

  if (m_GenerateUpdates) {
    QDateTime latest = m_PreviousUpdate;

    foreach(QFileInfo file, files) {
      QDateTime mod = file.lastModified();
      if (mod > m_PreviousUpdate) {
        m_BrowserModel->updatedFile(file);
      }

      if (mod > latest) {
        latest = mod;
      }
    }

    m_PreviousUpdate = latest;
  }

  int           column = m_BrowserModel->sortedColumn();
  Qt::SortOrder order  = m_BrowserModel->sortOrder();

  bool (*lt)(QFileInfo f1, QFileInfo f2) = NULL;

  switch(column) {
  case 0:
    if (order == Qt::AscendingOrder) {
      lt = fileNameLessThan;
    } else {
      lt = fileNameGreaterThan;
    }
    break;

  case 1:
    if (order == Qt::AscendingOrder) {
      lt = fileSizeLessThan;
    } else {
      lt = fileSizeGreaterThan;
    }
    break;

  case 2:
    if (order == Qt::AscendingOrder) {
      lt = fileDateLessThan;
    } else {
      lt = fileDateGreaterThan;
    }
    break;
  }

  if (lt) {
    try {
      qStableSort(m_Directories.begin(), m_Directories.end(), fileNameLessThan);
      qStableSort(m_Files.begin(), m_Files.end(), lt);
    }

    catch (...) {
      if (g_Application && qcepDebug(DEBUG_BROWSER)) {
        g_Application->printMessage("Sorting abandoned");
      }

      return;
    }
  }

  if (g_Application && qcepDebug(DEBUG_BROWSER)) {
    g_Application->printMessage(tr("Sort file browser took %1 msec").arg(tic.elapsed()));
  }

  int limit = 10000;

  if (g_Application) {
    limit = g_Application->get_FileBrowserLimit();
  }

  int trueSize = m_Files.count();

  if (limit && limit < trueSize) {
    m_BrowserModel->newDataAvailable(m_Directories, m_Files.mid(0,limit), limit, trueSize);
  } else {
    m_BrowserModel->newDataAvailable(m_Directories, m_Files);
  }
}

void QxrdFileBrowserModelUpdater::generateFileUpdates(int doIt)
{
  if (doIt) {
    m_PreviousUpdate = QDateTime::currentDateTime();
  }

  m_GenerateUpdates = doIt;
}
