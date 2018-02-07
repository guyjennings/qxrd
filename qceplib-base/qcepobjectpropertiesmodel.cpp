#include "qcepobjectpropertiesmodel.h"

QcepObjectPropertiesModel::QcepObjectPropertiesModel(QObject *parent, QcepObjectWPtr obj)
  : QAbstractItemModel(parent)
{
}

QVariant QcepObjectPropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  // TODO: Implement me!

  return QVariant();
}

QModelIndex QcepObjectPropertiesModel::index(int row, int column, const QModelIndex &parent) const
{
  // TODO: Implement me!

  return QModelIndex();
}

QModelIndex QcepObjectPropertiesModel::parent(const QModelIndex &index) const
{
  // TODO: Implement me!

  return QModelIndex();
}

int QcepObjectPropertiesModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return 0;

  // TODO: Implement me!

  return 0;
}

int QcepObjectPropertiesModel::columnCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return 0;

  // TODO: Implement me!

  return 0;
}

QVariant QcepObjectPropertiesModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  // TODO: Implement me!
  return QVariant();
}
