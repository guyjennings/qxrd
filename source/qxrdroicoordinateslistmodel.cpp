#include "qxrdroicoordinateslistmodel.h"
#include "qxrdroicoordinateslist.h"
#include "qxrdroicoordinates.h"

QxrdROICoordinatesListModel::QxrdROICoordinatesListModel(QxrdROICoordinatesListWPtr coords)
  : QAbstractListModel(),
    m_ROICoordinates(coords)
{
}

int QxrdROICoordinatesListModel::rowCount(const QModelIndex &parent) const
{
  QxrdROICoordinatesListPtr coords(m_ROICoordinates);

  if (coords) {
    return coords->get_RoiCount();
  } else {
    return 0;
  }
}

int QxrdROICoordinatesListModel::columnCount(const QModelIndex &parent) const
{
  QxrdROICoordinatesListPtr coords(m_ROICoordinates);

  if (coords) {
    return 6;
  } else {
    return 0;
  }
}

QVariant QxrdROICoordinatesListModel::data(const QModelIndex &index, int role) const
{
  int row = index.row();
  int col = index.column();

  QxrdROICoordinatesPtr c = roi(row);

  if (c) {
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
      if (col == 0) {
        return row;
      } else if (col == 1) {
        return c->get_RoiTypeName();
      } else if (col == 2) {
        return c->get_Coords().left();
      } else if (col == 3) {
        return c->get_Coords().top();
      } else if (col == 4) {
        return c->get_Coords().right();
      } else if (col == 5) {
        return c->get_Coords().bottom();
      }
    }
  }

  return QVariant();
}

QVariant QxrdROICoordinatesListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      if (section == 0) {
        return "#";
      } else if (section == 1) {
        return "Type";
      } else if (section == 2) {
        return "Left";
      } else if (section == 3) {
        return "Top";
      } else if (section == 4) {
        return "Right";
      } else if (section == 5) {
        return "Bottom";
      }
    } else if (role == Qt::TextAlignmentRole) {
      return Qt::AlignHCenter;
    }
  }

  return QVariant();
}

Qt::ItemFlags QxrdROICoordinatesListModel::flags(const QModelIndex &index) const
{
  return QAbstractListModel::flags(index);
}

bool QxrdROICoordinatesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  return false;
}

void QxrdROICoordinatesListModel::append(QxrdROICoordinatesPtr coords)
{
  QxrdROICoordinatesListPtr rois(m_ROICoordinates);

  if (rois) {
    beginInsertRows(QModelIndex(), rois->get_RoiCount(), rois->get_RoiCount());

    rois->appendROI(coords);

    endInsertRows();

//    renumberROIs();
  }
}

void QxrdROICoordinatesListModel::removeROI(int row)
{
  QxrdROICoordinatesListPtr rois(m_ROICoordinates);

  if (rois) {
    beginRemoveRows(QModelIndex(), row, row);

    rois->removeROI(row);

    endRemoveRows();

//    renumberROIs();
  }
}

void QxrdROICoordinatesListModel::moveROIDown(int row)
{
}

void QxrdROICoordinatesListModel::moveROIUp(int row)
{
}

void QxrdROICoordinatesListModel::editROI(int row)
{
}

QxrdROICoordinatesPtr QxrdROICoordinatesListModel::roi(int row) const
{
  QxrdROICoordinatesListPtr coords(m_ROICoordinates);
  QxrdROICoordinatesPtr res;

  if (coords) {
    res = coords->roi(row);
  }

  return res;
}
