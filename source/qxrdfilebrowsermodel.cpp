#include "qxrdfilebrowsermodel.h"
#include <QTime>
#include <stdio.h>
#include <QStringList>
#include <QDirIterator>
#include <QSize>
#include <QPixmap>
#include "qcepdebug.h"
#include "qxrdapplication.h"
#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdfilebrowsermodelupdaterthread.h"
#include "qxrdmutexlocker.h"

QxrdFileBrowserModel::QxrdFileBrowserModel(QObject *parent) :
  QAbstractTableModel(parent),
  m_Mutex(QMutex::Recursive),
  m_UpdaterThread(NULL),
  m_Updater(NULL),
  m_SortedColumn(0),
  m_SortOrder(Qt::AscendingOrder),
  m_Limit(1000),
  m_TrueSize(0),
  m_HighlightOnTime(5),
  m_HighlightFadeTime(25),
  m_HighlightSaturation(200),
  m_HighlightHue(116)
{
}

QxrdFileBrowserModel::~QxrdFileBrowserModel()
{
  if (m_UpdaterThread) {
    m_UpdaterThread->shutdown();
  }
}

void QxrdFileBrowserModel::initialize(QxrdFileBrowserModelWPtr model)
{
  m_Model = model;
  m_UpdaterThread =
      QxrdFileBrowserModelUpdaterThreadPtr(
        new QxrdFileBrowserModelUpdaterThread());
  m_UpdaterThread -> setModel(m_Model);
  m_UpdaterThread -> setObjectName("browser");
  m_UpdaterThread -> start();
  m_Updater = m_UpdaterThread->updater();
}

QVariant QxrdFileBrowserModel::headerData
  (int section, Qt::Orientation orientation, int role) const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
    case 0:
      return "File name";

    case 1:
      return "Size";

    case 2:
      return "Modified";

    default:
      return "";
    }
  } else {
    return inherited::headerData(section, orientation, role);
  }
}

QVariant QxrdFileBrowserModel::data(const QModelIndex &idx, int role) const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QModelIndex index = idx;
  QFileInfo info = fileInfo(index);

  if (role == Qt::DisplayRole) {
    if (index.isValid()) {
      if (index.column() == 0) {
        return info.fileName();
      } else if (index.column() == 1) {
        if (info.isDir()) {
          return "--";
        } else if (!info.exists()) {
          return "";
        } else {
          qint64 sz = info.size();

          if (sz < 1024) {
            return tr("%1 B").arg(sz);
          } else if (sz < 1024*1024) {
            return tr("%1 KB").arg(sz/1024);
          } else if (sz < 1024*1024*1024) {
            return tr("%1 MB").arg(sz/(1024*1024));
          } else {
            return tr("%1 GB").arg(sz/(1024*1024*1024));
          }
        }
      } else if (index.column() == 2) {
        return info.lastModified();
      }
    }
  } else if (role == Qt::DecorationRole) {
      if (index.column() == 0) {
        if (info.isDir()) {
          return QPixmap(":/images/folder-16x16.png");
        } else if (!info.exists()) {
          return QVariant();
        } else {
          QString suffix = info.suffix();

          if (suffix == "metadata") {
            return QPixmap(":/images/file-metadata-16x16.png");
          } else if (suffix == "avg") {
            return QPixmap(":/images/file-integration-16x16.png");
          } else if (suffix == "tif") {
            return QPixmap(":/images/file-image-16x16.png");
          } else {
            return QPixmap(":/images/file-generic-16x16.png");
          }
        }
      }
      //  } else if (role == Qt::SizeHintRole) {
      //    return QSize(80,10);
  } else if (role == Qt::BackgroundRole) {
    int lastMod = info.lastModified().secsTo(QDateTime::currentDateTime());

    if (lastMod > (m_HighlightOnTime+m_HighlightFadeTime)) {
      return QColor(Qt::white);
    } else {
      m_Updater -> needUpdate();

      if (lastMod > m_HighlightOnTime) {
        double fade = lastMod - m_HighlightOnTime;

        return QColor::fromHsv(m_HighlightHue, int(m_HighlightSaturation*double(m_HighlightFadeTime-fade)/double(m_HighlightFadeTime)), 255, 255);
      } else {
        return QColor::fromHsv(m_HighlightHue, m_HighlightSaturation, 255, 255);
      }
    }
  }

  return QVariant();
}

int	QxrdFileBrowserModel::columnCount ( const QModelIndex & /*parent*/ ) const
{
  return 3;
}

int	QxrdFileBrowserModel::rowCount ( const QModelIndex & /*parent*/ ) const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_Limit > 0) {
    return m_DirList.count() + m_Limit + 1;
  } else {
    return m_FileList.count() + m_DirList.count();
  }
}

void QxrdFileBrowserModel::setNameFilters(QStringList filters)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_NameFilters = filters;

  updateModel();
}

void QxrdFileBrowserModel::setNameFilterDisables(bool /*disables*/)
{
}

QFileInfo QxrdFileBrowserModel::fileInfo(const QModelIndex &index) const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  int n = index.row();
  QFileInfo info;

  if (n >= m_DirList.count()) {
    int nf = n-m_DirList.count();

    if (nf >= m_FileList.count()) {
      info = QFileInfo(tr("... %1 additional files not displayed...").arg(m_TrueSize-m_Limit));
    } else {
      info = m_FileList.at(nf);
    }
  } else {
    info = m_DirList.at(n);
  }

  return info;
}

QString QxrdFileBrowserModel::fileName(const QModelIndex &index) const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return fileInfo(index).fileName();
}

QString QxrdFileBrowserModel::filePath(const QModelIndex &index) const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return fileInfo(index).filePath();
}

void QxrdFileBrowserModel::setRootPath(QString path)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_RootPath = path;

  updateModel();

  emit rootChanged(m_RootPath);
}

QStringList QxrdFileBrowserModel::nameFilters() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_NameFilters;
}

int QxrdFileBrowserModel::sortedColumn() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_SortedColumn;
}

Qt::SortOrder QxrdFileBrowserModel::sortOrder() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_SortOrder;
}

void QxrdFileBrowserModel::refresh()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  updateModel();
}

void QxrdFileBrowserModel::updateModel()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Updater->needUpdate();
}

bool QxrdFileBrowserModel::isDir(const QModelIndex &index) const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return fileInfo(index).isDir();
}

void QxrdFileBrowserModel::sort (int column, Qt::SortOrder order)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if ((m_SortedColumn != column) || (m_SortOrder != order)) {
    m_SortedColumn = column;
    m_SortOrder    = order;

    m_Updater -> needUpdate();
  }
}

void QxrdFileBrowserModel::newDataAvailable(QVector<QFileInfo> dirs, QVector<QFileInfo> files, int limit, int trueSize)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  beginResetModel();

  m_Limit = limit;
  m_TrueSize = trueSize;
  m_DirList = dirs;
  m_FileList = files;

  endResetModel();
}

void QxrdFileBrowserModel::updatedFile(QFileInfo file)
{
//  QxrdExperimentPtr expt(m_Experiment);

//  if (expt && qcepDebug(DEBUG_BROWSER)) {
//    expt->printMessage(tr("file %1 updated at %2")
//                                .arg(path)
//                                .arg(atTime.toString(Qt::ISODate)));
//  }

  emit fileUpdated(file);
}

void QxrdFileBrowserModel::generateFileUpdates(int doIt)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Updater->generateFileUpdates(doIt);
}
