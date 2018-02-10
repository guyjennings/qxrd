#include "qcepobjecttreemodel.h"
#include "qcepobject.h"
#include "qcepdebug.h"

QcepObjectTreeModel::QcepObjectTreeModel(QObject *parent, QcepObjectWPtr obj)
  : QAbstractItemModel(parent),
    m_Object(obj)
{
}

void QcepObjectTreeModel::printMessage(QString msg, QDateTime dt) const
{
  QcepObjectPtr obj(m_Object);

  if (obj) {
    obj->printMessage(msg, dt);
  }
}

QcepObjectPtr QcepObjectTreeModel::indexedObject(const QModelIndex& index) const
{
  QcepObjectPtr res;

  if (index.isValid()) {
    QcepObject *obj =
        static_cast<QcepObject*>(index.internalPointer());

    if (obj) {
      res = obj->sharedFromThis();
    }
  } else {
    res = m_Object;
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

  if (qcepDebug(DEBUG_OBJECTBROWSER)) {
    printMessage(tr("QcepObjectTreeModel::index(%1,%2,%3 [%4]) = %5")
                 .arg(row).arg(column)
                 .arg(parentObject?parentObject->objectName():"NULL")
                 .HEXARG(parentObject.data())
                 .arg(indexDescription(res)));
  }

  return res;
}

QModelIndex QcepObjectTreeModel::parent(const QModelIndex &index) const
{
  QModelIndex res = QModelIndex();

  QcepObjectPtr object = indexedObject(index);

  if (object) {
    QcepObjectPtr p(object->parentPtr());

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
  } else {
    QcepObjectPtr root(m_Object);

    res = root->childCount();
  }

  if (qcepDebug(DEBUG_OBJECTBROWSER)) {
    printMessage(tr("QcepObjectTreeModel::rowCount(%1) = %2").arg(indexDescription(parent)).arg(res));
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

QString QcepObjectTreeModel::indexDescription(const QModelIndex &index) const
{
  if (index.isValid()) {
    QcepObjectPtr obj = indexedObject(index);
    QcepObjectPtr par = indexedObject(parent(index));

    if (obj) {
      if (par) {
        return tr("\"%1\" : Row %2, Col %3 of \"%4\"")
            .arg(obj->objectName())
            .arg(index.row())
            .arg(index.column())
            .arg(par->objectName());
      } else {
        return tr("\"%1\" : Row %2, Col %3 of \"/\"")
            .arg(obj->objectName())
            .arg(index.row())
            .arg(index.column());
      }
    }
  }

  return tr("(%1,%2,null)").arg(index.row()).arg(index.column());
}

