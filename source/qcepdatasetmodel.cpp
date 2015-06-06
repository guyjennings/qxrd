#include "qcepdatasetmodel.h"
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepdataset.h"

QcepDatasetModel::QcepDatasetModel(QcepDatasetPtr ds) :
  m_Dataset(ds)
{

}

QcepDataObjectPtr QcepDatasetModel::indexedObject(const QModelIndex &index) const
{
  if (index.isValid()) {
    QcepDataObject *obj = static_cast<QcepDataObject*>(index.internalPointer());

    if (obj) {
      return QcepDataObjectPtr(obj);
    }
  }

  return QcepDataObjectPtr();
}

QModelIndex QcepDatasetModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent)) {
    return QModelIndex();
  } else {
    QcepDataObjectPtr parentItem = indexedObject(parent);

    if (parentItem) {
      QcepDataObjectPtr childItem = parentItem->item(row);

      if (childItem) {
        return createIndex(row, column, childItem.data());
      }
    }
  }

  return QModelIndex();
}

QModelIndex QcepDatasetModel::parent(const QModelIndex &index) const
{
  if (!index.isValid()) {
    return QModelIndex();
  } else {
    QcepDataObjectPtr childItem = indexedObject(index);

    if (childItem) {
      QcepDataObjectPtr parentItem = childItem->parentItem();

      if (parentItem) {
        if (parentItem == m_Dataset) {
          return QModelIndex();
        } else {
          return createIndex(parentItem->indexInParent(), 0, parentItem.data());
        }
      }
    }
  }

  return QModelIndex();
}

int QcepDatasetModel::rowCount(const QModelIndex &parent) const
{
  if (parent.column() > 0) {
    return 0;
  }

  QcepDataObjectPtr parentItem;

  if (!parent.isValid()) {
    parentItem = m_Dataset;
  } else {
    parentItem = indexedObject(parent);
  }

  if (parentItem) {
    return parentItem->count();
  }

  return 0;
}

int QcepDatasetModel::columnCount(const QModelIndex &parent) const
{
  QcepDataObjectPtr parentItem;

  if (!parent.isValid()) {
    parentItem = m_Dataset;
  } else {
    parentItem = indexedObject(parent);
  }

  if (parentItem) {
    return parentItem->columnCount();
  }

  return 0;
}

QVariant QcepDatasetModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }

  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  QcepDataObjectPtr object = indexedObject(index);

  if (object) {
    return object->columnData(index.column());
  }

  return QVariant();
}
