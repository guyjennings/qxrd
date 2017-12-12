#include "qxrddetectorlistmodel.h"
#include "qxrddetectorsettings.h"

QxrdDetectorListModel::QxrdDetectorListModel()
{
}

int QxrdDetectorListModel::rowCount(const QModelIndex & /*parent*/) const
{
  return m_Detectors.count();
}

int QxrdDetectorListModel::columnCount(const QModelIndex & /*parent*/) const
{
  return 4;
}

QVariant QxrdDetectorListModel::data(const QModelIndex &index, int role) const
{
  int row = index.row();
  int col = index.column();
  QxrdDetectorSettingsPtr p = m_Detectors.value(row);

  if (p) {
    if (role == Qt::EditRole || role == Qt::DisplayRole) {
      if (col == 0) {
        return row;
      } else if (col == 2) {
        return p->get_DetectorTypeName();
      } else if (col == 3) {
        return p->get_DetectorName();
      }
    } else if (role == Qt::CheckStateRole) {
      if (col == 1) {
        if (p->get_Enabled()) {
          return Qt::Checked;
        } else {
          return Qt::Unchecked;
        }
      }
    } else if (role == Qt::TextAlignmentRole) {
      if (col == 0 || col == 1) {
        return Qt::AlignHCenter;
      } else {
        return Qt::AlignAbsolute;
      }
    }
  }

  return QVariant();
}

QVariant QxrdDetectorListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      if (section == 0) {
        return "#";
      }

      if (section == 1) {
        return "Enabled";
      }

      if (section == 2) {
        return "Type";
      }

      if (section == 3) {
        return "Name";
      }
    } else if (role == Qt::TextAlignmentRole) {
      return Qt::AlignHCenter;
    }
  }

  return QVariant();
}

bool QxrdDetectorListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  int row = index.row();
  int col = index.column();

  QxrdDetectorSettingsPtr proxy = m_Detectors.value(row);

  if (proxy) {
    if (col == 1 && role == Qt::CheckStateRole) {
      proxy->set_Enabled(!proxy->get_Enabled());

      emit dataChanged(index, index);

      return true;
    } else if (col == 3) {
      if (role == Qt::EditRole || role == Qt::DisplayRole) {
        proxy->set_DetectorName(value.toString());

        emit dataChanged(index, index);

        return true;
      }
    }
  }

  return false;
}

Qt::ItemFlags QxrdDetectorListModel::flags(const QModelIndex &index) const
{
//  int row = index.row();
  int col = index.column();

  if (col == 1) {
    return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable;
  } else if (col == 3) {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
  } else {
    return QAbstractListModel::flags(index);
  }
}

void QxrdDetectorListModel::append(QxrdDetectorSettingsPtr det)
{
  beginInsertRows(QModelIndex(), m_Detectors.count(), m_Detectors.count());

  m_Detectors.append(det);

  endInsertRows();
}

QxrdDetectorSettingsPtr QxrdDetectorListModel::detector(int i)
{
  return m_Detectors.value(i);
}

void QxrdDetectorListModel::removeDetector(int row)
{
  beginRemoveRows(QModelIndex(), row, row);

  m_Detectors.remove(row);

  endRemoveRows();
}

void QxrdDetectorListModel::moveDetectorDown(int row)
{
  int nRows = m_Detectors.count();

  if (row >= 0 && row < (nRows-1)) {
    beginMoveRows(QModelIndex(), row+1, row+1, QModelIndex(), row);

    QxrdDetectorSettingsPtr p1 = m_Detectors[row];
    QxrdDetectorSettingsPtr p2 = m_Detectors[row+1];

    m_Detectors[row] = p2;
    m_Detectors[row+1] = p1;

    endMoveRows();
  }
}

void QxrdDetectorListModel::moveDetectorUp(int row)
{
  int nRows = m_Detectors.count();

  if (row >= 1 && row < nRows) {
    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row-1);

    QxrdDetectorSettingsPtr p1 = m_Detectors[row-1];
    QxrdDetectorSettingsPtr p2 = m_Detectors[row];

    m_Detectors[row-1] = p2;
    m_Detectors[row] = p1;

    endMoveRows();
  }
}

void QxrdDetectorListModel::configureDetector(int row)
{
  QxrdDetectorSettingsPtr p = m_Detectors.value(row);

  if (p) {
    p->configureDetector();
  }
}
