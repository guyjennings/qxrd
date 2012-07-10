#include "qxrddebug.h"
#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdapplication.h"
#include <QThread>
#include <QDirIterator>
#include "qxrdfilebrowsermodel.h"

QxrdFileBrowserModelUpdater::QxrdFileBrowserModelUpdater(QxrdFileBrowserModelPtr browser, QObject *parent) :
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

  connect(m_BrowserModel.data(), SIGNAL(rootChanged(const QString&)),
          this,                  SLOT(changeRoot(const QString&)));
  connect(m_FileSystemWatcher,   SIGNAL(directoryChanged(const QString&)),
          this,                  SLOT(changeContents(const QString&)), Qt::DirectConnection);
  connect(&m_UpdateTimer,        SIGNAL(timeout()),
          this,                  SLOT(updateTimeout()));

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

  if (path != "") {
    m_FileSystemWatcher->addPath(path);
    m_RootPath = path;
  }
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

bool QxrdFileBrowserModelUpdater::updateNeeded()
{
  return m_UpdateNeeded.fetchAndAddOrdered(0);
}

class QxrdSortInterruptChecker {
public:
  QxrdSortInterruptChecker(QxrdFileBrowserModelUpdaterWPtr updater);

  void sortInterruptCheck();

private:
  QxrdFileBrowserModelUpdaterWPtr m_Updater;
};

QxrdSortInterruptChecker::QxrdSortInterruptChecker(QxrdFileBrowserModelUpdaterWPtr updater)
  : m_Updater(updater)
{
}

void QxrdSortInterruptChecker::sortInterruptCheck()
{
  QxrdFileBrowserModelUpdaterPtr updater(m_Updater);

  if (updater && updater->updateNeeded()) {
    throw 0;
  }
}

class QxrdFileNameLessThan : public QxrdSortInterruptChecker {
public:
  QxrdFileNameLessThan(QxrdFileBrowserModelUpdaterWPtr updater);

  bool operator()(QFileInfo f1, QFileInfo f2);
};

QxrdFileNameLessThan::QxrdFileNameLessThan(QxrdFileBrowserModelUpdaterWPtr updater)
  : QxrdSortInterruptChecker(updater)
{
}

bool QxrdFileNameLessThan::operator()(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.fileName().toLower() < f2.fileName().toLower();
}

class QxrdFileNameGreaterThan : public QxrdSortInterruptChecker {
public:
  QxrdFileNameGreaterThan(QxrdFileBrowserModelUpdaterWPtr updater);

  bool operator()(QFileInfo f1, QFileInfo f2);
};

QxrdFileNameGreaterThan::QxrdFileNameGreaterThan(QxrdFileBrowserModelUpdaterWPtr updater)
  : QxrdSortInterruptChecker(updater)
{
}

bool QxrdFileNameGreaterThan::operator()(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.fileName().toLower() > f2.fileName().toLower();
}

class QxrdFileSizeLessThan : public QxrdSortInterruptChecker {
public:
  QxrdFileSizeLessThan(QxrdFileBrowserModelUpdaterWPtr updater);

  bool operator()(QFileInfo f1, QFileInfo f2);
};

QxrdFileSizeLessThan::QxrdFileSizeLessThan(QxrdFileBrowserModelUpdaterWPtr updater)
  : QxrdSortInterruptChecker(updater)
{
}

bool QxrdFileSizeLessThan::operator()(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.size() < f2.size();
}

class QxrdFileSizeGreaterThan : public QxrdSortInterruptChecker {
public:
  QxrdFileSizeGreaterThan(QxrdFileBrowserModelUpdaterWPtr updater);

  bool operator()(QFileInfo f1, QFileInfo f2);
};

QxrdFileSizeGreaterThan::QxrdFileSizeGreaterThan(QxrdFileBrowserModelUpdaterWPtr updater)
  : QxrdSortInterruptChecker(updater)
{
}

bool QxrdFileSizeGreaterThan::operator()(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.size() > f2.size();
}

class QxrdFileDateLessThan : public QxrdSortInterruptChecker {
public:
  QxrdFileDateLessThan(QxrdFileBrowserModelUpdaterWPtr updater);

  bool operator()(QFileInfo f1, QFileInfo f2);
};

QxrdFileDateLessThan::QxrdFileDateLessThan(QxrdFileBrowserModelUpdaterWPtr updater)
  : QxrdSortInterruptChecker(updater)
{
}

bool QxrdFileDateLessThan::operator()(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.lastModified() < f2.lastModified();
}

class QxrdFileDateGreaterThan : public QxrdSortInterruptChecker {
public:
  QxrdFileDateGreaterThan(QxrdFileBrowserModelUpdaterWPtr updater);

  bool operator()(QFileInfo f1, QFileInfo f2);
};

QxrdFileDateGreaterThan::QxrdFileDateGreaterThan(QxrdFileBrowserModelUpdaterWPtr updater)
  : QxrdSortInterruptChecker(updater)
{
}

bool QxrdFileDateGreaterThan::operator ()(QFileInfo f1, QFileInfo f2)
{
  sortInterruptCheck();

  return f1.lastModified() > f2.lastModified();
}

void QxrdFileBrowserModelUpdater::updateContents()
{
  QTime tic;
  tic.start();

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

  try {
    qStableSort(m_Directories.begin(), m_Directories.end(),
                QxrdFileNameLessThan(this));

    switch(column) {
    case 0:
      if (order == Qt::AscendingOrder) {
        qStableSort(m_Files.begin(), m_Files.end(),
                    QxrdFileNameLessThan(this));
      } else {
        qStableSort(m_Files.begin(), m_Files.end(),
                    QxrdFileNameGreaterThan(this));
      }
      break;

    case 1:
      if (order == Qt::AscendingOrder) {
        qStableSort(m_Files.begin(), m_Files.end(),
                    QxrdFileSizeLessThan(this));
      } else {
        qStableSort(m_Files.begin(), m_Files.end(),
                    QxrdFileSizeGreaterThan(this));
      }
      break;

    case 2:
      if (order == Qt::AscendingOrder) {
        qStableSort(m_Files.begin(), m_Files.end(),
                    QxrdFileDateLessThan(this));
      } else {
        qStableSort(m_Files.begin(), m_Files.end(),
                    QxrdFileDateGreaterThan(this));
      }
      break;
    }
  }

  catch (...) {
    if (g_Application && qcepDebug(DEBUG_BROWSER)) {
      g_Application->printMessage("Sorting abandoned");
    }

    return;
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
