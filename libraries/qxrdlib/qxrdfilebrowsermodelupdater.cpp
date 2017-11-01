#include "qxrddebug.h"
#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdapplicationsettings.h"
#include <QThread>
#include <QDirIterator>
#include "qxrdfilebrowsermodel.h"
#include "qxrdfilebrowsermodelupdaterthread.h"
#include "qxrdfilebrowsermodelupdaterthread-ptr.h"

QxrdFileBrowserModelUpdater::QxrdFileBrowserModelUpdater(QString name) :
  QcepObject(name),
  m_BrowserModel(),
  m_RootPath(""),
  m_FileSystemWatcher(NULL),
  m_UpdateNeeded(1),
  m_UpdateTimer(),
  m_UpdateInterval(1000),
  m_PreviousUpdate(QDateTime::currentDateTime()),
  m_GenerateUpdates(true)
{
  if (qcepDebug(DEBUG_BROWSER)) {
    printMessage("Starting Browser Model Updater");
  }
}

void QxrdFileBrowserModelUpdater::initialize(QxrdFileBrowserModelWPtr browser, QcepObjectWPtr parent)
{
  setBrowserModel(browser);
}

void QxrdFileBrowserModelUpdater::setBrowserModel(QxrdFileBrowserModelWPtr browser)
{
  m_BrowserModel = browser;

  delete m_FileSystemWatcher;

  m_FileSystemWatcher = new QFileSystemWatcher(this);

  QxrdFileBrowserModelPtr model(m_BrowserModel);

  if (model) {
    connect(model.data(),        &QxrdFileBrowserModel::rootChanged,
            this,                &QxrdFileBrowserModelUpdater::changeRoot);
  }

  connect(m_FileSystemWatcher,   &QFileSystemWatcher::directoryChanged,
          this,                  &QxrdFileBrowserModelUpdater::changeContents, Qt::DirectConnection);
  connect(&m_UpdateTimer,        &QTimer::timeout,
          this,                  &QxrdFileBrowserModelUpdater::updateTimeout);

  m_UpdateTimer.setSingleShot(true);
  m_UpdateTimer.start(m_UpdateInterval);
}

QxrdFileBrowserModelUpdater::~QxrdFileBrowserModelUpdater()
{
#ifndef QT_NO_DEBUG
  printf("Deleting file browser model updater\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileBrowserModelUpdater::~QxrdFileBrowserModelUpdater(%p)\n", this);
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdFileBrowserModelUpdater::~QxrdFileBrowserModelUpdater");
  }
}

void QxrdFileBrowserModelUpdater::changeRoot(const QString& path)
{
  if (qcepDebug(DEBUG_BROWSER)) {
    printMessage(tr("QxrdFileBrowserModelUpdater::changeRoot %1").arg(path));
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
  if (qcepDebug(DEBUG_BROWSER)) {
    printMessage(tr("QxrdFileBrowserModelUpdater::changeContents %1").arg(path));
  }

  m_UpdateNeeded.fetchAndStoreOrdered(1);
}

void QxrdFileBrowserModelUpdater::updateTimeout()
{
  if (m_UpdateNeeded.fetchAndStoreOrdered(0)) {
    if (qcepDebug(DEBUG_BROWSER)) {
      printMessage(tr("QxrdFileBrowserModelUpdater::updateTimeout update needed %1").arg(m_RootPath));
    }

    updateContents();
  }

  m_UpdateTimer.start(m_UpdateInterval);
}

bool QxrdFileBrowserModelUpdater::updateNeeded()
{
  return m_UpdateNeeded.fetchAndAddOrdered(0);
}

static void checkSortInterrupt()
{
  QThread *curr = QThread::currentThread();

  QxrdFileBrowserModelUpdaterThread *upthrd =
      qobject_cast<QxrdFileBrowserModelUpdaterThread*>(curr);

  if (upthrd) {
    QxrdFileBrowserModelUpdaterPtr updater
        = upthrd->updater();

    if (updater && updater->updateNeeded()) {
      throw 0;
    }
  }
}

static bool fileNameLessThan(QFileInfo f1, QFileInfo f2)
{
  checkSortInterrupt();

  return f1.fileName().toLower() < f2.fileName().toLower();
}

static bool fileNameGreaterThan(QFileInfo f1, QFileInfo f2)
{
  checkSortInterrupt();

  return f1.fileName().toLower() > f2.fileName().toLower();
}

static bool fileSizeLessThan(QFileInfo f1, QFileInfo f2)
{
  checkSortInterrupt();

  return f1.size() < f2.size();
}

static bool fileSizeGreaterThan(QFileInfo f1, QFileInfo f2)
{
  checkSortInterrupt();

  return f1.size() > f2.size();
}

static bool fileDateLessThan(QFileInfo f1, QFileInfo f2)
{
  checkSortInterrupt();

  return f1.lastModified() < f2.lastModified();
}

static bool fileDateGreaterThan(QFileInfo f1, QFileInfo f2)
{
  checkSortInterrupt();

  return f1.lastModified() > f2.lastModified();
}

void QxrdFileBrowserModelUpdater::updateContents()
{
  QTime tic;
  tic.start();

  m_UpdateNeeded.fetchAndStoreOrdered(0);

  QxrdFileBrowserModelPtr model(m_BrowserModel);

  if (model) {
    QDirIterator iterd(m_RootPath);
    QDirIterator iter(m_RootPath, model->nameFilters());
    QVector<QFileInfo> dirs;
    QVector<QFileInfo> files;

    while (iterd.hasNext()) {
      if (m_UpdateNeeded.fetchAndAddOrdered(0)) {
        if (qcepDebug(DEBUG_BROWSER)) {
          printMessage("Directory list generation abandoned");
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
        if (qcepDebug(DEBUG_BROWSER)) {
          printMessage("File list generation abandoned");
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

    if (qcepDebug(DEBUG_BROWSER)) {
      printMessage(tr("Update file browser took %1 msec").arg(tic.restart()));
      printMessage(tr("File Path %1: %2 dirs, %3 files")
                   .arg(m_RootPath).arg(m_Directories.count()).arg(m_Files.count()));
    }

    if (m_GenerateUpdates.fetchAndAddOrdered(0)) {
      QDateTime latest = m_PreviousUpdate;

      foreach(QFileInfo file, files) {
        QDateTime mod = file.lastModified();
        if (mod > m_PreviousUpdate) {
          model->updatedFile(file);
        }

        if (mod > latest) {
          latest = mod;
        }
      }

      m_PreviousUpdate = latest;
    }

    int           column = model->sortedColumn();
    Qt::SortOrder order  = model->sortOrder();

    try {
      qStableSort(m_Directories.begin(), m_Directories.end(),
                  fileNameLessThan);

      switch(column) {
      case 0:
        if (order == Qt::AscendingOrder) {
          qStableSort(m_Files.begin(), m_Files.end(),
                      fileNameLessThan);
        } else {
          qStableSort(m_Files.begin(), m_Files.end(),
                      fileNameGreaterThan);
        }
        break;

      case 1:
        if (order == Qt::AscendingOrder) {
          qStableSort(m_Files.begin(), m_Files.end(),
                      fileSizeLessThan);
        } else {
          qStableSort(m_Files.begin(), m_Files.end(),
                      fileSizeGreaterThan);
        }
        break;

      case 2:
        if (order == Qt::AscendingOrder) {
          qStableSort(m_Files.begin(), m_Files.end(),
                      fileDateLessThan);
        } else {
          qStableSort(m_Files.begin(), m_Files.end(),
                      fileDateGreaterThan);
        }
        break;
      }
    }

    catch (...) {
      if (qcepDebug(DEBUG_BROWSER)) {
        printMessage("Sorting abandoned");
      }

      return;
    }

    if (qcepDebug(DEBUG_BROWSER)) {
      printMessage(tr("Sort file browser took %1 msec").arg(tic.elapsed()));
    }

    int limit = 10000;

    QxrdApplicationSettings *app = qobject_cast<QxrdApplicationSettings*>(g_ApplicationSettings);

    if (app) {
      limit = app->get_FileBrowserLimit();
    }

    int trueSize = m_Files.count();

    if (limit && limit < trueSize) {
      model->newDataAvailable(m_Directories, m_Files.mid(0,limit), limit, trueSize);
    } else {
      model->newDataAvailable(m_Directories, m_Files);
    }
  }
}

void QxrdFileBrowserModelUpdater::generateFileUpdates(int doIt)
{
  if (doIt) {
    m_PreviousUpdate = QDateTime::currentDateTime();
  }

  m_GenerateUpdates = doIt;
}