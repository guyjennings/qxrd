#include "qcepdataobjectpropertiesmodel.h"
#include "qcepdataobject.h"
#include <QObject>
#include <QMetaProperty>

QcepDataObjectPropertiesModel::QcepDataObjectPropertiesModel(QcepDataObjectWPtr obj) :
  m_Object(obj)
{

}

int QcepDataObjectPropertiesModel::rowCount(const QModelIndex &parent) const
{
  return 2 + propertyCount() + dynamicPropertyCount();
}

int QcepDataObjectPropertiesModel::columnCount(const QModelIndex &parent) const
{
  return 2;
}

QVariant QcepDataObjectPropertiesModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();
  int pc = propertyCount();
  int dpc = dynamicPropertyCount();
  QcepDataObjectPtr obj(m_Object);

  if (role == Qt::DisplayRole) {
    if (index.isValid()) {
      int r = index.row(), c = index.column();

      if (r == 0) {
        if (c == 0) {
          res = "Static";
        } else if (c == 1) {
          res = "Properties";
        }
      } else if (r < (pc+1)) {
        if (obj) {
          QMetaProperty mp = obj->metaObject()->property(r-1);
          if (c == 0) {
            res = mp.name();
          } else if (c == 1) {
            res = obj->property(mp.name());
          }
        }
      } else if (r == (pc+1)) {
        if (c == 0) {
          res = "Dynamic";
        } else if (c == 1) {
          res = "Properties";
        }
      } else {
        int idx = r - pc - 2;
        if (obj) {
          QString name = obj->dynamicPropertyNames().value(idx);

          if (c == 0) {
            res = name;
          } else if (c == 1) {
            res = obj->property(qPrintable(name));
          }
        }
      }
    }
  }

  return res;
}

QVariant QcepDataObjectPropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      if (section == 0) {
        res = "Name";
      } else if (section == 1) {
        res = "Value";
      }
    } else if (orientation == Qt::Vertical) {
      int pc = propertyCount();
      int dpc = dynamicPropertyCount();

      if (section == 0) {

      } else if (section < (pc+1)) {
        return section - 1;
      } else if (section == (pc+1)) {

      } else {
        return section - pc - 2;
      }
    }
  }

  return res;
}

int QcepDataObjectPropertiesModel::propertyCount() const
{
  QcepDataObjectPtr obj(m_Object);

  if (obj) {
    return obj->metaObject()->propertyCount();
  } else {
    return 0;
  }
}

int QcepDataObjectPropertiesModel::dynamicPropertyCount() const
{
  QcepDataObjectPtr obj(m_Object);

  if (obj) {
    return obj->dynamicPropertyNames().count();
  } else {
    return 0;
  }
}
