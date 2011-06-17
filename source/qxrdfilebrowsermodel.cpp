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

QxrdFileBrowserModel::QxrdFileBrowserModel(QObject *parent) :
  QAbstractTableModel(parent),
  m_UpdaterThread(NULL),
  m_Updater(NULL),
  m_SortedColumn(0),
  m_SortOrder(Qt::AscendingOrder),
  m_Limit(0),
  m_TrueSize(0)
{
  m_UpdaterThread = new QxrdFileBrowserModelUpdaterThread(this);
  m_UpdaterThread -> start();
  m_Updater = m_UpdaterThread->updater();
}

QxrdFileBrowserModel::~QxrdFileBrowserModel()
{
  if (m_UpdaterThread) {
    m_UpdaterThread->shutdown();
  }
}

QVariant QxrdFileBrowserModel::headerData
  (int section, Qt::Orientation orientation, int role) const
{
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
  }

  return QVariant();
}

int	QxrdFileBrowserModel::columnCount ( const QModelIndex & parent ) const
{
  return 3;
}

int	QxrdFileBrowserModel::rowCount ( const QModelIndex & parent ) const
{
  if (m_Limit > 0) {
    return m_DirList.count() + m_Limit + 1;
  } else {
    return m_FileList.count() + m_DirList.count();
  }
}

void QxrdFileBrowserModel::setNameFilters(QStringList filters)
{
  m_NameFilters = filters;

  updateModel();
}

void QxrdFileBrowserModel::setNameFilterDisables(bool disables)
{
}

QFileInfo QxrdFileBrowserModel::fileInfo(const QModelIndex &index) const
{
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
  return fileInfo(index).fileName();
}

QString QxrdFileBrowserModel::filePath(const QModelIndex &index) const
{
  return fileInfo(index).filePath();
}

void QxrdFileBrowserModel::setRootPath(QString path)
{
  m_RootPath = path;

  updateModel();

  emit rootChanged(m_RootPath);
}

QStringList QxrdFileBrowserModel::nameFilters() const
{
  return m_NameFilters;
}

int QxrdFileBrowserModel::sortedColumn() const
{
  return m_SortedColumn;
}

Qt::SortOrder QxrdFileBrowserModel::sortOrder() const
{
  return m_SortOrder;
}

void QxrdFileBrowserModel::refresh()
{
  updateModel();
}

void QxrdFileBrowserModel::updateModel()
{
  m_Updater->needUpdate();
}

bool QxrdFileBrowserModel::isDir(const QModelIndex &index) const
{
  return fileInfo(index).isDir();
}

void QxrdFileBrowserModel::sort (int column, Qt::SortOrder order)
{
  if ((m_SortedColumn != column) || (m_SortOrder != order)) {
    m_SortedColumn = column;
    m_SortOrder    = order;

    m_Updater -> needUpdate();
  }
}

void QxrdFileBrowserModel::newDataAvailable(QVector<QFileInfo> dirs, QVector<QFileInfo> files, int limit, int trueSize)
{
  beginResetModel();

  m_Limit = limit;
  m_TrueSize = trueSize;
  m_DirList = dirs;
  m_FileList = files;

  endResetModel();
}
