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
        return c->get_Coords().center().x();
      } else if (col == 3) {
        return c->get_Coords().center().y();
      } else if (col == 4) {
        return c->get_Coords().width();
      } else if (col == 5) {
        return c->get_Coords().height();
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
        return "CenterX";
      } else if (section == 3) {
        return "CenterY";
      } else if (section == 4) {
        return "Width";
      } else if (section == 5) {
        return "Height";
      }
    } else if (role == Qt::TextAlignmentRole) {
      return Qt::AlignHCenter;
    }
  }

  return QVariant();
}

Qt::ItemFlags QxrdROICoordinatesListModel::flags(const QModelIndex &index) const
{
  int row = index.row();
  int col = index.column();

  if (col == 2 || col == 3 || col == 4 || col == 5) {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
  } else {
    return QAbstractListModel::flags(index);
  }
}

bool QxrdROICoordinatesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  int row = index.row();
  int col = index.column();

  QxrdROICoordinatesPtr c = roi(row);

  if (c) {
    if (role == Qt::EditRole || role == Qt::DisplayRole) {
      if (col == 2) {
        c->setCenterX(value.toDouble());
      } else if (col == 3) {
        c->setCenterY(value.toDouble());
      } else if (col == 4) {
        c->setWidth(value.toDouble());
      } else if (col == 5) {
        c->setHeight(value.toDouble());
      } else {
        return false;
      }

      setRoi(row, c);

      emit dataChanged(index, index);

      return true;
    }
  }
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
  QxrdROICoordinatesListPtr rois(m_ROICoordinates);

  if (rois) {
    int nRows = rois->get_RoiCount();

    if (row >= 0 && row < (nRows-1)) {
      beginMoveRows(QModelIndex(), row+1, row+1, QModelIndex(), row);

      QxrdROICoordinatesPtr p1 = rois->roi(row);
      QxrdROICoordinatesPtr p2 = rois->roi(row+1);

      rois->setRoi(row, p2);
      rois->setRoi(row+1, p1);

      endMoveRows();

//      renumberROIs();
    }
  }
}

void QxrdROICoordinatesListModel::moveROIUp(int row)
{
  QxrdROICoordinatesListPtr rois(m_ROICoordinates);

  if (rois) {
    int nRows = rois->get_RoiCount();

    if (row >= 1 && row < nRows) {
      beginMoveRows(QModelIndex(), row, row, QModelIndex(), row-1);

      QxrdROICoordinatesPtr p1 = rois->roi(row-1);
      QxrdROICoordinatesPtr p2 = rois->roi(row);

      rois->setRoi(row-1, p2);
      rois->setRoi(row, p1);

      endMoveRows();

//      renumberROIs();
    }
  }
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

void QxrdROICoordinatesListModel::setRoi(int row, QxrdROICoordinatesPtr c)
{
  QxrdROICoordinatesListPtr coords(m_ROICoordinates);

  if (coords) {
    coords->setRoi(row, c);
  }
}
