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
  m_SortOrder(Qt::AscendingOrder)
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
  return m_FileList.count() + m_DirList.count();
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
    info = m_FileList.at(n-m_DirList.count());
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

void QxrdFileBrowserModel::refresh()
{
  updateModel();
}

void QxrdFileBrowserModel::updateModel()
{
  QTime tic;
  tic.start();

  beginResetModel();

  QDirIterator iterd(m_RootPath);
  QDirIterator iter(m_RootPath, m_NameFilters);
  QVector<QFileInfo> dirs;
  QVector<QFileInfo> files;

  while (iterd.hasNext()) {
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
    QString filePath = iter.next();
    QFileInfo fileInfo(m_RootPath, filePath);

    if (fileInfo.isDir()) {
    } else {
      files.append(fileInfo);
    }
  }

  m_DirList  = dirs;
  m_FileList = files;

  endResetModel();

  if (qcepDebug(DEBUG_DISPLAY)) {
    g_Application->printMessage(tr("Update file browser took %1 msec").arg(tic.elapsed()));
    g_Application->printMessage(tr("File Path %1: %2 dirs, %3 files")
                                .arg(m_RootPath).arg(m_DirList.count()).arg(m_FileList.count()));
  }

  sort(m_SortedColumn, m_SortOrder);
}

bool QxrdFileBrowserModel::isDir(const QModelIndex &index) const
{
  return fileInfo(index).isDir();
}

bool fileNameLessThan(QFileInfo f1, QFileInfo f2)
{
  return f1.fileName().toLower() < f2.fileName().toLower();
}

bool fileNameGreaterThan(QFileInfo f1, QFileInfo f2)
{
  return f1.fileName().toLower() > f2.fileName().toLower();
}

bool fileSizeLessThan(QFileInfo f1, QFileInfo f2)
{
  return f1.size() < f2.size();
}

bool fileSizeGreaterThan(QFileInfo f1, QFileInfo f2)
{
  return f1.size() > f2.size();
}

bool fileDateLessThan(QFileInfo f1, QFileInfo f2)
{
  return f1.lastModified() < f2.lastModified();
}

bool fileDateGreaterThan(QFileInfo f1, QFileInfo f2)
{
  return f1.lastModified() > f2.lastModified();
}

void QxrdFileBrowserModel::sort (int column, Qt::SortOrder order)
{
  QTime tic;
  tic.start();

  beginResetModel();

  m_SortedColumn = column;
  m_SortOrder = order;

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
    qStableSort(m_DirList.begin(), m_DirList.end(), fileNameLessThan);
    qStableSort(m_FileList.begin(), m_FileList.end(), lt);
  }

  endResetModel();

  if (qcepDebug(DEBUG_DISPLAY)) {
    g_Application->printMessage(tr("Sort file browser took %1 msec").arg(tic.elapsed()));
  }
}
