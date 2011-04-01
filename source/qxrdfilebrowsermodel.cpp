#include "qxrdfilebrowsermodel.h"
#include <QTime>
#include <stdio.h>
#include <QStringList>
#include <QDirIterator>

QxrdFileBrowserModel::QxrdFileBrowserModel(QObject *parent) :
  QAbstractItemModel(parent)
{
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
    }
  }

  return QVariant();
}

QVariant QxrdFileBrowserModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    if (index.isValid()) {
      QFileInfo info = m_FileList.at(index.row());

      if (index.column() == 0) {
        return info.fileName();
      } else if (index.column() == 1) {
        return info.size();
      } else if (index.column() == 2) {
        return info.lastModified();
      }
    }
  }

  return QVariant();
}

int	QxrdFileBrowserModel::columnCount ( const QModelIndex & parent ) const
{
//  if (parent.isValid()) {
//    return 3;
//  } else {
    return 3;
//  }
}

int	QxrdFileBrowserModel::rowCount ( const QModelIndex & parent ) const
{
//  if (parent.isValid()) {
//    return 1;
//  } else {
    return m_FileList.count();
//  }
}

QModelIndex QxrdFileBrowserModel::parent(const QModelIndex &child) const
{
//  if (!child.isValid()) {
    return QModelIndex();
//  }


}

QModelIndex QxrdFileBrowserModel::index(QString path) const
{
  if (m_RootPath.canonicalFilePath() == path) {
    return index(0,0);
  }

  for (int i=0; i<rowCount(); i++) {
    if (m_FileList.at(i).canonicalFilePath() == path) {
      return index(i,0);
    }
  }

  return QModelIndex();
}

QModelIndex QxrdFileBrowserModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent)) {
    return QModelIndex();
  }

  if (row >= 0 && row < m_FileList.count() && column >= 0 && column < 3) {
    return createIndex(row, column);
  }

  return QModelIndex();
}

void QxrdFileBrowserModel::setNameFilters(QStringList filters)
{
}

void QxrdFileBrowserModel::setNameFilterDisables(bool disables)
{
}

QString QxrdFileBrowserModel::fileName(const QModelIndex &index)
{
  if (index.isValid()) {
    return m_FileList.at(index.row()).fileName();
  }

  return "xxx";
}

QString QxrdFileBrowserModel::filePath(const QModelIndex &index) const
{
  if (index.isValid()) {
    return m_FileList.at(index.row()).filePath();
  }

  return "/xxx/yyy";
}

void QxrdFileBrowserModel::setRootPath(QString path)
{
  beginResetModel();

  QDirIterator iter(path);
  QVector<QFileInfo> res;

  while (iter.hasNext()) {
    res.append(QFileInfo(path, iter.next()));
  }

  m_RootPath = QFileInfo(path);
  m_FileList = res;

  endResetModel();
}

bool QxrdFileBrowserModel::isDir(const QModelIndex &index) const
{
  if (index.isValid()) {
    return m_FileList.at(index.row()).isDir();
  }

  return false;
}

bool QxrdFileBrowserModel::hasChildren (const QModelIndex &index) const
{
  return false;
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
  beginResetModel();

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
    qStableSort(m_FileList.begin(), m_FileList.end(), lt);
  }

  endResetModel();
}

//QxrdFileBrowserModel::QxrdFileBrowserModel(QObject *parent) :
//    QFileSystemModel(parent)
//{
//}

//QVariant QxrdFileBrowserModel::data(const QModelIndex &index, int role) const
//{
//  QTime tic;
//  tic.start();

//  QVariant result = QFileSystemModel::data(index, role);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::data took %d msec\n", elapsed);
//  }

//  return result;
//}

//bool	QxrdFileBrowserModel::canFetchMore ( const QModelIndex & parent ) const
//{
//  QTime tic;
//  tic.start();

//  bool result = QFileSystemModel::canFetchMore(parent);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::canFetchMore took %d msec\n", elapsed);
//  }

//  return result;
//}

//int	QxrdFileBrowserModel::columnCount ( const QModelIndex & parent ) const
//{
//  QTime tic;
//  tic.start();

//  int result = QFileSystemModel::columnCount(parent);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::columnCount took %d msec\n", elapsed);
//  }

//  return result;
//}

//bool	QxrdFileBrowserModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
//{
//  QTime tic;
//  tic.start();

//  bool result = QFileSystemModel::dropMimeData(data, action, row, column, parent);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::dropMimeData took %d msec\n", elapsed);
//  }

//  return result;
//}

//void	QxrdFileBrowserModel::fetchMore ( const QModelIndex & parent )
//{
//  QTime tic;
//  tic.start();

//  QFileSystemModel::fetchMore(parent);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::fetchMore took %d msec\n", elapsed);
//  }
//}

//Qt::ItemFlags	QxrdFileBrowserModel::flags ( const QModelIndex & index ) const
//{
//  QTime tic;
//  tic.start();

//  Qt::ItemFlags result = QFileSystemModel::flags(index);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::flags took %d msec\n", elapsed);
//  }

//  return result;
//}

//bool	QxrdFileBrowserModel::hasChildren ( const QModelIndex & parent ) const
//{
//  QTime tic;
//  tic.start();

//  bool result = QFileSystemModel::hasChildren(parent);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::hasChildren took %d msec\n", elapsed);
//  }

//  return result;
//}

//QVariant	QxrdFileBrowserModel::headerData ( int section, Qt::Orientation orientation, int role ) const
//{
//  QTime tic;
//  tic.start();

//  QVariant result = QFileSystemModel::headerData(section, orientation, role);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::headerData took %d msec\n", elapsed);
//  }

//  return result;
//}

//QModelIndex	QxrdFileBrowserModel::index ( const QString & path, int column ) const
//{
//  QTime tic;
//  tic.start();

//  QModelIndex result = QFileSystemModel::index(path, column);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::index took %d msec\n", elapsed);
//  }

//  return result;
//}

//QModelIndex	QxrdFileBrowserModel::index ( int row, int column, const QModelIndex & parent ) const
//{
//  QTime tic;
//  tic.start();

//  QModelIndex result = QFileSystemModel::index(row, column, parent);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::index took %d msec\n", elapsed);
//  }

//  return result;
//}

//QMimeData *	QxrdFileBrowserModel::mimeData ( const QModelIndexList & indexes ) const
//{
//  QTime tic;
//  tic.start();

//  QMimeData * result = QFileSystemModel::mimeData(indexes);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::mimeData took %d msec\n", elapsed);
//  }

//  return result;
//}

//QStringList	QxrdFileBrowserModel::mimeTypes () const
//{
//  QTime tic;
//  tic.start();

//  QStringList result = QFileSystemModel::mimeTypes();

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::mimeTypes took %d msec\n", elapsed);
//  }

//  return result;
//}

//QModelIndex	QxrdFileBrowserModel::parent ( const QModelIndex & index ) const
//{
//  QTime tic;
//  tic.start();

//  QModelIndex result = QFileSystemModel::parent(index);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::parent took %d msec\n", elapsed);
//  }

//  return result;
//}

//int	QxrdFileBrowserModel::rowCount ( const QModelIndex & parent ) const
//{
//  QTime tic;
//  tic.start();

//  int result = QFileSystemModel::rowCount(parent);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::rowCount took %d msec\n", elapsed);
//  }

////  printf("QxrdFileBrowserModel::rowCount = %d\n", result);

//  return result;
//}

//bool	QxrdFileBrowserModel::setData ( const QModelIndex & idx, const QVariant & value, int role )
//{
//  QTime tic;
//  tic.start();

//  bool result = QFileSystemModel::setData(idx, value, role);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::setData took %d msec\n", elapsed);
//  }

//  return result;
//}

//void	QxrdFileBrowserModel::sort ( int column, Qt::SortOrder order )
//{
//  QTime tic;
//  tic.start();

//  QFileSystemModel::sort(column, order);

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::sort took %d msec\n", elapsed);
//  }
//}

//Qt::DropActions	QxrdFileBrowserModel::supportedDropActions () const
//{
//  QTime tic;
//  tic.start();

//  Qt::DropActions result = QFileSystemModel::supportedDropActions();

//  int elapsed = tic.elapsed();

//  if (elapsed > 1000) {
//    printf("QxrdFileBrowserModel::supportedDropActions took %d msec\n", elapsed);
//  }

//  return result;
//}
