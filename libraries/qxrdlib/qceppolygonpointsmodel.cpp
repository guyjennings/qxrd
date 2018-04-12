#include "qceppolygonpointsmodel.h"
#include <QtMath>
#include <QItemSelectionModel>

QcepPolygonPointsModel::QcepPolygonPointsModel(QPolygonF poly, QObject *parent)
  : QAbstractTableModel(parent),
    m_Polygon(poly)
{
}

int QcepPolygonPointsModel::rowCount(const QModelIndex & /*parent*/) const
{
//  if (!parent.isValid())
//    return 0;

  return m_Polygon.count();
}

int QcepPolygonPointsModel::columnCount(const QModelIndex & /*parent*/) const
{
//  if (!parent.isValid())
//    return 0;

  return ColCount;
}

QVariant QcepPolygonPointsModel::data(const QModelIndex &index, int role) const
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
      if (row < m_Polygon.count()) {
        double v = m_Polygon.value(row).x();

        if (v == v) {
          return v;
        }
      }
      break;

    case YCol:
      if (row < m_Polygon.count()) {
        double v = m_Polygon.value(row).y();

        if (v == v) {
          return v;
        }
      }
      break;
    }
  }

  return QVariant();
}

QVariant QcepPolygonPointsModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool QcepPolygonPointsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  int row = index.row();
  int col = index.column();

  if (role == Qt::EditRole || role == Qt::DisplayRole) {
    if (col == XCol) {
      setPoly(row, XCol, value.toDouble());
    } else if (col == YCol) {
      setPoly(row, YCol, value.toDouble());
    }

    emit dataChanged(index, index);

    return true;
  }

  return false;
}

Qt::ItemFlags QcepPolygonPointsModel::flags(const QModelIndex &index) const
{
  int col = index.column();

  if (col == XCol || col == YCol) {
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
  } else {
    return QAbstractTableModel::flags(index);
  }
}

void QcepPolygonPointsModel::setPoly(int n, int col, double v)
{
  if (n >= 0 && n < m_Polygon.count()) {
    if (col == XCol) {
      QPointF& a = m_Polygon[n];
      a.setX(v);
    } else if (col == YCol) {
      m_Polygon[n].setY(v);
    }
  } else if (n == m_Polygon.count()) {
    beginInsertRows(QModelIndex(), n, n);

    if (col == XCol) {
      m_Polygon.append(QPointF(v,qQNaN()));
    } else if (col == YCol) {
      m_Polygon.append(QPointF(qQNaN(),v));
    }

    endInsertRows();
  }
}

QPointF QcepPolygonPointsModel::polygonPoint(int i)
{
  return m_Polygon.value(i);
}

void QcepPolygonPointsModel::addPoint(QItemSelectionModel *sel)
{
  if (sel) {
    int nDone = 0;
    for (int i=m_Polygon.count(); i>=0; i--) {
      if (sel->rowIntersectsSelection(i, QModelIndex())) {
        nDone++;

        beginInsertRows(QModelIndex(), i+1, i+1);
        m_Polygon.insert(i+1, QPointF(qQNaN(), qQNaN()));
        endInsertRows();
      }
    }

    if (nDone == 0) {
      beginInsertRows(QModelIndex(), m_Polygon.count(), m_Polygon.count());
      m_Polygon.append(QPointF(qQNaN(), qQNaN()));
      endInsertRows();
    }
  }
}

void QcepPolygonPointsModel::delPoint(QItemSelectionModel *sel)
{
  if (sel) {
    for (int i=m_Polygon.count(); i>=0; i--) {
      if (sel->rowIntersectsSelection(i, QModelIndex())) {
        beginRemoveRows(QModelIndex(), i,i);
        m_Polygon.remove(i);
        endRemoveRows();
      }
    }
  }
}
