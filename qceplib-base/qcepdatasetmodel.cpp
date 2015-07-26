#include "qcepdatasetmodel.h"
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepdataset.h"
#include "qcepdebug.h"
#include <QMimeData>
#include <stdio.h>

QcepDatasetModel::QcepDatasetModel(QcepDatasetWPtr ds) :
  m_Dataset(ds)
{
  connect(ds.data(), SIGNAL(dataObjectChanged()), this, SLOT(onDataObjectChanged()));
}

QcepDataObjectPtr QcepDatasetModel::indexedObject(const QModelIndex &index) const
{
  QcepDataObjectPtr res;

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::indexedObject(%s)\n", qPrintable(indexDescription(index)));
  }

  if (index.isValid()) {
    QcepDataObject *obj = static_cast<QcepDataObject*>(index.internalPointer());

    if (obj) {
//      if (qcepDebug(DEBUG_DATABROWSER)) {
//        printf("of object %s\n", qPrintable(obj->get_Name()));
//      }

      res = obj->sharedFromThis();

      if (!res) {
        printf("QcepDatasetModel::indexedObject returns NULL\n");
      }
    }
//  } else {
//    res = NULL;
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::indexedObject(%s)", qPrintable(indexDescription(index)));
    if (res) {
      printf(" = %s\n", qPrintable(res->get_Name()));
    } else {
      printf(" = null\n");
    }
  }

  return res;
}

QModelIndex QcepDatasetModel::index(int row, int column, const QModelIndex &parent) const
{
  QModelIndex res = QModelIndex();

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::index(%d,%d,%s)\n",
           row, column, qPrintable(indexDescription(parent)));
  }

//  if (hasIndex(row, column, parent)) {
  QcepDataObjectPtr parentItem = indexedObject(parent);

  if (!parentItem) {
    parentItem = m_Dataset;
  }

  if (parentItem) {
    QcepDataObjectPtr childItem = parentItem->item(row);

    if (childItem) {
      res = createIndex(row, column, childItem.data());
    }
  }
  //  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::index(%d,%d,%s) = %s\n", row, column,
           qPrintable(indexDescription(parent)),
           qPrintable(indexDescription(res)));
  }

  return res;
}

QModelIndex QcepDatasetModel::parent(const QModelIndex &index) const
{
  QModelIndex res = QModelIndex();

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::parent(%s)\n", qPrintable(indexDescription(index)));
  }

  QcepDataObjectPtr childItem = indexedObject(index);

  if (childItem) {
//    if (qcepDebug(DEBUG_DATABROWSER)) {
//      printf("of object %s\n", qPrintable(childItem->get_Name()));
//    }

    QcepDataObjectPtr parentItem = childItem->parentItem();

    if (parentItem) {
      if (parentItem != m_Dataset.data()) {
        res = createIndex(parentItem->indexInParent(), 0, parentItem.data());
      }
    }
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::parent(%s)", qPrintable(indexDescription(index)));
    printf(" = (%s)\n", qPrintable(indexDescription(res)));
  }

  return res;
}

int QcepDatasetModel::rowCount(const QModelIndex &parent) const
{
  int res = 0;

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::rowCount(%s)\n", qPrintable(indexDescription(parent)));
  }

  if (parent.column() <= 0) {
    QcepDataObjectPtr parentItem = indexedObject(parent);

    if (parentItem) {
      int nrows = parentItem->childCount();

      res = nrows;
    } else {
      QcepDatasetPtr ds(m_Dataset);

      if (ds) {
        res = ds->childCount();
      }
    }
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::rowCount(%s)", qPrintable(indexDescription(parent)));
    printf(" = %d\n", res);
  }

  return res;
}

int QcepDatasetModel::columnCount(const QModelIndex &parent) const
{
  int res = 0;

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::columnCount(%s)\n", qPrintable(indexDescription(parent)));
  }

  QcepDataObjectPtr parentItem = indexedObject(parent);

  if (parentItem) {
    res = 3;
  } else {
    res = 3;
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::columnCount(%s)", qPrintable(indexDescription(parent)));
    printf(" = %d\n", res);
  }

  return res;
}

QVariant QcepDatasetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation==Qt::Horizontal) {
    if (role==Qt::DisplayRole) {
      switch (section) {
      case 0:
        return "Name";

      case 1:
        return "Type";

      case 2:
        return "Description";
      }
    }
  }

  return QVariant();
}

QVariant QcepDatasetModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::data(%s,%d)\n", qPrintable(indexDescription(index)), role);
  }

  if (!index.isValid()) {
    res = QVariant();
  }  else {

    QcepDataObjectPtr object = indexedObject(index);

    if (object) {
      if (role == Qt::DisplayRole) {
        res = object->columnData(index.column());
      } else if (role == Qt::ToolTipRole) {
        res = object->pathName() + "\n" +
            object->columnData(1).toString() + "\n" +
            object->columnData(2).toString();
      }
    }
  }

  if (qcepDebug(DEBUG_DATABROWSER)) {
    printf("QcepDatasetModel::data(%s,%d)", qPrintable(indexDescription(index)), role);
    printf(" = %s\n", qPrintable(res.toString()));
  }

  return res;
}

Qt::ItemFlags QcepDatasetModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

  if (index.isValid()) {
    return defaultFlags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
  } else {
    return defaultFlags | Qt::ItemIsDropEnabled;
  }
}

QStringList QcepDatasetModel::mimeTypes() const
{
//  if (qcepDebug(DEBUG_DRAGDROP)) {
    printf("QcepDatasetModel::mimeTypes\n");
//  }

  QStringList types;
  types << "application/vnd.text.list";
//  types << "text/plain";
  return types;
}

QMimeData  *QcepDatasetModel::mimeData(const QModelIndexList &indexes) const
{
  QMimeData *mimeData = new QMimeData();
  QString textData;

  foreach (const QModelIndex &index, indexes) {
      if (index.isValid()) {
          QString text = data(index, Qt::DisplayRole).toString();
          textData += text;
      }
  }

  if (qcepDebug(DEBUG_DRAGDROP)) {
    printf("QcepDatasetModel::mimeData = %s\n", qPrintable(textData));
  }

  mimeData->setText(textData);
//  mimeData->setData("text/plain", encodedData);
  return mimeData;
}

QString QcepDatasetModel::indexDescription(const QModelIndex &index) const
{
  if (index.isValid()) {
    QcepDataObject* obj = static_cast<QcepDataObject*>(index.internalPointer());

    if (obj) {
      return tr("(%1,%2,\"%3\")")
          .arg(index.row()).arg(index.column())
          .arg(obj->get_Name());
    } else {
      QcepDatasetPtr ds(m_Dataset);

      if (ds) {
        return tr("(%1,%2,\"%3\")")
            .arg(index.row()).arg(index.column())
            .arg(ds->get_Name());
      } else {
        return "";
      }
    }
  }

  return tr("(%1,%2,null)").arg(index.row()).arg(index.column());
}

bool QcepDatasetModel::insertRows(int row, int count, const QModelIndex &parent)
{
  printf("QcepDatasetModel::insertRows(row:%d, count:%d, parent:%s)\n", row, count, qPrintable(indexDescription(parent)));

  return QAbstractItemModel::insertRows(row, count, parent);
}

bool QcepDatasetModel::moveRows
  (const QModelIndex &sourceParent, int sourceRow, int count,
   const QModelIndex &destinationParent, int destinationChild)
{
  printf("QAbstractItemModel::moveRows(sourceParent:%s, sourceRow:%d, count:%d,\n"
         "                             destinationParent:%s, destinationChild:%d)\n",
         qPrintable(indexDescription(sourceParent)), sourceRow, count,
         qPrintable(indexDescription(destinationParent)), destinationChild);

  return QAbstractItemModel::moveRows(sourceParent, sourceRow, count,
                                      destinationParent, destinationChild);
}

bool QcepDatasetModel::removeRows(int row, int count, const QModelIndex &parent)
{
  printf("QcepDatasetModel::removeRows(row:%d, count:%d, parent:%s)\n", row, count, qPrintable(indexDescription(parent)));

  return QAbstractItemModel::removeRows(row, count, parent);
}

void QcepDatasetModel::onDataObjectChanged()
{
  beginResetModel();
  endResetModel();
}

QcepDataObjectPtr      QcepDatasetModel::item(const QModelIndex &index)
{
  printf("QcepDatasetModel::item([%d,%d])\n", index.row(), index.column());

  return QcepDataObjectPtr();
}

QcepDataObjectPtr      QcepDatasetModel::item(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->item(path);
  } else {
    return QcepDataObjectPtr();
  }
}

QcepDataGroupPtr       QcepDatasetModel::group(const QModelIndex &index)
{
}

QcepDataGroupPtr       QcepDatasetModel::group(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->group(path);
  } else {
    return QcepDataGroupPtr();
  }
}

QcepDataGroupPtr       QcepDatasetModel::newGroup(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->newGroup(path);
  } else {
    return QcepDataGroupPtr();
  }
}

QcepDataArrayPtr       QcepDatasetModel::array(const QModelIndex &index)
{
}

QcepDataArrayPtr       QcepDatasetModel::array(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->array(path);
  } else {
    return QcepDataArrayPtr();
  }
}

QcepDataArrayPtr       QcepDatasetModel::newArray(QString path, QVector<int> dims)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->newArray(path, dims);
  } else {
    return QcepDataArrayPtr();
  }
}

QcepDataColumnPtr      QcepDatasetModel::column(const QModelIndex &index)
{
}

QcepDataColumnPtr      QcepDatasetModel::column(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->column(path);
  } else {
    return QcepDataColumnPtr();
  }
}

QcepDataColumnPtr      QcepDatasetModel::newColumn(QString path, int nRows)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->newColumn(path, nRows);
  } else {
    return QcepDataColumnPtr();
  }
}

QcepDataColumnScanPtr  QcepDatasetModel::columnScan(const QModelIndex &index)
{
}

QcepDataColumnScanPtr  QcepDatasetModel::columnScan(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->columnScan(path);
  } else {
    return QcepDataColumnScanPtr();
  }
}

QcepDataColumnScanPtr  QcepDatasetModel::newColumnScan(QString path, int nRows, QStringList cols)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->newColumnScan(path, nRows, cols);
  } else {
    return QcepDataColumnScanPtr();
  }
}

QcepDoubleImageDataPtr QcepDatasetModel::image(const QModelIndex &index)
{
}

QcepDoubleImageDataPtr QcepDatasetModel::image(QString path)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->image(path);
  } else {
    return QcepDoubleImageDataPtr();
  }
}

QcepDoubleImageDataPtr QcepDatasetModel::newImage(QString path, int width, int height)
{
  QcepDatasetPtr ds(m_Dataset);

  if (ds) {
    return ds->newImage(path, width, height);
  } else {
    return QcepDoubleImageDataPtr();
  }
}

void                   QcepDatasetModel::append(const QModelIndex &index, QcepDataObjectPtr obj)
{
}

void                   QcepDatasetModel::append(QString path, QcepDataObjectPtr obj)
{
}

void                   QcepDatasetModel::remove(const QModelIndex &index)
{
}

void                   QcepDatasetModel::remove(QString path)
{
}
