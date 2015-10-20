#include "qxrdroicoordinateslistmodel.h"
#include "qxrdroicoordinateslist.h"
#include "qxrdroicoordinates.h"

QxrdROICoordinatesListModel::QxrdROICoordinatesListModel(QxrdROICoordinatesListWPtr coords)
  : QAbstractListModel(),
    m_ROICoordinates(coords)
{
  QxrdROICoordinatesListPtr crds(m_ROICoordinates);

  if (crds) {
    connect(crds.data(), &QxrdROICoordinatesList::roiChanged, this, &QxrdROICoordinatesListModel::onROIChanged);
    connect(crds.data(), &QxrdROICoordinatesList::roisChanged, this, &QxrdROICoordinatesListModel::onROIsChanged);
  }
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
    return ColCount;
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
      if (col == NumCol) {
        return row;
      } else if (col == ValueCol) {
        return c->get_Value();
      } else if (col == TypeCol) {
        return c->get_RoiTypeName();
      } else if (col == CenterXCol) {
        return c->get_Coords().center().x();
      } else if (col == CenterYCol) {
        return c->get_Coords().center().y();
      } else if (col == WidthCol) {
        return c->get_Coords().width();
      } else if (col == HeightCol) {
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
      if (section == NumCol) {
        return "#";
      } else if (section == ValueCol) {
        return "Value";
      } else if (section == TypeCol) {
        return "Type";
      } else if (section == CenterXCol) {
        return "CenterX";
      } else if (section == CenterYCol) {
        return "CenterY";
      } else if (section == WidthCol) {
        return "Width";
      } else if (section == HeightCol) {
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

  if (col == TypeCol ||
      col == CenterXCol ||
      col == CenterYCol ||
      col == WidthCol ||
      col == HeightCol) {
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
      if (col == ValueCol) {
        c->set_Value(value.toDouble());
      } else if (col == TypeCol) {
        c->selectNamedROIType(value.toString());
      } else if (col == CenterXCol) {
        c->setCenterX(value.toDouble());
      } else if (col == CenterYCol) {
        c->setCenterY(value.toDouble());
      } else if (col == WidthCol) {
        c->setWidth(value.toDouble());
      } else if (col == HeightCol) {
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
  }
}

void QxrdROICoordinatesListModel::removeROI(int row)
{
  QxrdROICoordinatesListPtr rois(m_ROICoordinates);

  if (rois) {
    beginRemoveRows(QModelIndex(), row, row);

    rois->removeROI(row);

    endRemoveRows();
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

void QxrdROICoordinatesListModel::moveROICenter(int i, double x, double y)
{
  QxrdROICoordinatesListPtr coords(m_ROICoordinates);

  if (coords) {
    QxrdROICoordinatesPtr roi = coords->roi(i);

    if (roi) {
      roi->setCenter(QPointF(x,y));

      emit dataChanged(index(i,0), index(i,columnCount(QModelIndex())));
    }
  }
}

void QxrdROICoordinatesListModel::onROIChanged(int i)
{
  emit dataChanged(index(i,0), index(i,columnCount(QModelIndex())));
}

void QxrdROICoordinatesListModel::onROIsChanged()
{
  emit dataChanged(index(0,0), index(rowCount(QModelIndex()), columnCount(QModelIndex())));
}
