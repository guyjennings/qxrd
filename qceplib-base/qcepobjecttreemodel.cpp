#include "qcepobjecttreemodel.h"

QcepObjectTreeModel::QcepObjectTreeModel(QObject *parent, QcepObjectWPtr obj)
  : QAbstractItemModel(parent),
    m_Object(obj)
{
}

QVariant QcepObjectTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  // TODO: Implement me!

  return QVariant();
}

QModelIndex QcepObjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  // TODO: Implement me!

  return QModelIndex();
}

QModelIndex QcepObjectTreeModel::parent(const QModelIndex &index) const
{
  // TODO: Implement me!

  return QModelIndex();
}

int QcepObjectTreeModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return 0;

  // TODO: Implement me!

  return 0;
}

int QcepObjectTreeModel::columnCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return 0;

  // TODO: Implement me!

  return 0;
}

QVariant QcepObjectTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  // TODO: Implement me!
  return QVariant();
}
