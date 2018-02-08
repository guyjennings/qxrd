#include "qcepobjecttreemodel.h"
#include "qcepobject.h"

QcepObjectTreeModel::QcepObjectTreeModel(QObject *parent, QcepObjectWPtr obj)
  : QAbstractItemModel(parent),
    m_Object(obj)
{
}

QcepObjectPtr QcepObjectTreeModel::indexedObject(const QModelIndex& index) const
{
  QcepObjectPtr res;

  if (index.isValid()) {
    QcepObject *obj =
        static_cast<QcepObject*>(index.internalPointer());

    if (obj) {
      res = qSharedPointerDynamicCast<QcepObject>(obj->sharedFromThis());
    }
  }

  return res;
}

QModelIndex QcepObjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  QModelIndex res = QModelIndex();

  QcepObjectPtr parentObject = indexedObject(parent);

  if (!parentObject) {
    parentObject = m_Object;
  }

  if (parentObject) {
    QcepObjectPtr childItem = parentObject -> childPtr(row);

    if (childItem) {
      res = createIndex(row, column, childItem.data());
    }
  }

  return res;
}

QModelIndex QcepObjectTreeModel::parent(const QModelIndex &index) const
{
  QModelIndex res = QModelIndex();

  QcepObjectPtr object = indexedObject(index);

  if (object) {
    QcepObjectPtr p =
        qSharedPointerDynamicCast<QcepObject>(
          object->parentPtr());

    if (p != m_Object) {
      //TODO: check this
      res = createIndex(0 /*parent->indexInParent()*/, 0, p.data());
    }
  }

  return res;
}

int QcepObjectTreeModel::rowCount(const QModelIndex &parent) const
{
  int res = 0;

  if (parent.isValid()) {
    QcepObjectPtr item = indexedObject(parent);

    if (item) {
      res = item->childCount();
    } else {
      QcepObjectPtr root(m_Object);

      res = root->childCount();
    }
  }

  return res;
}

int QcepObjectTreeModel::columnCount(const QModelIndex &parent) const
{
//  int res = 0;

//  if (parent.isValid()) {
//    res = ColumnCount;
//  }

  return ColumnCount;
}

QVariant QcepObjectTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant res = QVariant();

  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
    case NameColumn:
      res = "Name";
      break;

    case ClassColumn:
      res = "Type";
      break;

    case AddressColumn:
      res = "Address";
      break;
    }
  }

  return res;
}

QVariant QcepObjectTreeModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  if (index.isValid()) {
    QcepObjectPtr object = indexedObject(index);

    if (object) {
      if (role == Qt::DisplayRole || role == Qt::EditRole){
        switch (index.column()) {
        case NameColumn:
          res = object->objectName();
          break;

        case ClassColumn:
          res = object->metaObject()->className();
          break;

        case AddressColumn:
          res = tr("%1").HEXARG(object.data());
          break;
        }
      }
    }
  }

  return res;
}
