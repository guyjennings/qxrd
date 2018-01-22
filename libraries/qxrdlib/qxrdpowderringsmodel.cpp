#include "qxrdpowderringsmodel.h"

QxrdPowderRingsModel::QxrdPowderRingsModel()
{

}

int QxrdPowderRingsModel::rowCount(const QModelIndex & /*parent*/) const
{
//  if (!parent.isValid())
//    return 0;

  return 10;
}

int QxrdPowderRingsModel::columnCount(const QModelIndex & /*parent*/) const
{
//  if (!parent.isValid())
//    return 0;

  return ColCount;
}

QVariant QxrdPowderRingsModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  int row = index.row();
//  int col = index.column();

  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    switch (index.column()) {
    case NumCol:
      return index.row();
      break;

    case XCol:
      if (row < rowCount()) {
        return row;
      }
      break;

    case YCol:
      if (row < rowCount()) {
        return row*row;
      }
      break;
    }
  }

  return QVariant();
}

QVariant QxrdPowderRingsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      if (section == NumCol) {
        return "#";
      } else if (section == XCol) {
        return "X";
      } else if (section == YCol) {
        return "Y";
      }
    } else if (role == Qt::TextAlignmentRole) {
      return Qt::AlignHCenter;
    }
  }

  return QVariant();
}

bool QxrdPowderRingsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  int row = index.row();
  int col = index.column();

  if (role == Qt::EditRole || role == Qt::DisplayRole) {
    if (col == XCol) {
//      setPoly(row, XCol, value.toDouble());
    } else if (col == YCol) {
//      setPoly(row, YCol, value.toDouble());
    }

    emit dataChanged(index, index);

    return true;
  }

  return false;
}

Qt::ItemFlags QxrdPowderRingsModel::flags(const QModelIndex &index) const
{
  int col = index.column();

  if (col == XCol || col == YCol) {
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
  } else {
    return QAbstractTableModel::flags(index);
  }
}
