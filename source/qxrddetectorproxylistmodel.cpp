#include "qxrddetectorproxylistmodel.h"
#include "qxrddetectorproxy.h"

QxrdDetectorProxyListModel::QxrdDetectorProxyListModel()
{
}

int QxrdDetectorProxyListModel::rowCount(const QModelIndex &parent) const
{
  return m_DetectorProxies.count();
}

int QxrdDetectorProxyListModel::columnCount(const QModelIndex &parent) const
{
  return 4;
}

QVariant QxrdDetectorProxyListModel::data(const QModelIndex &index, int role) const
{
  int row = index.row();
  int col = index.column();
  QxrdDetectorProxyPtr p = m_DetectorProxies.value(row);

  if (p) {
    if (role == Qt::EditRole || role == Qt::DisplayRole) {
      if (col == 0) {
        return row;
      } else if (col == 2) {
        return p->detectorTypeName();
      } else if (col == 3) {
        return p->detectorName();
      }
    } else if (role == Qt::CheckStateRole) {
      if (col == 1) {
        if (p->enabled()) {
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

QVariant QxrdDetectorProxyListModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool QxrdDetectorProxyListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  int row = index.row();
  int col = index.column();

  QxrdDetectorProxyPtr proxy = m_DetectorProxies.value(row);

  if (proxy) {
    if (col == 1 && role == Qt::CheckStateRole) {
      proxy->setEnabled(!proxy->enabled());

      emit dataChanged(index, index);

      return true;
    } else if (col == 3) {
      if (role == Qt::EditRole || role == Qt::DisplayRole) {
        proxy->setDetectorName(value.toString());

        emit dataChanged(index, index);

        return true;
      }
    }
  }

  return false;
}

Qt::ItemFlags QxrdDetectorProxyListModel::flags(const QModelIndex &index) const
{
  int row = index.row();
  int col = index.column();

  if (col == 1 || col == 3) {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
  } else {
    return QAbstractListModel::flags(index);
  }
}

void QxrdDetectorProxyListModel::append(QxrdDetectorProxyPtr proxy)
{
  beginInsertRows(QModelIndex(), m_DetectorProxies.count(), m_DetectorProxies.count());

  m_DetectorProxies.append(proxy);

  endInsertRows();
}

QxrdDetectorProxyPtr QxrdDetectorProxyListModel::detectorProxy(int i)
{
  return m_DetectorProxies.value(i);
}

void QxrdDetectorProxyListModel::removeDetector(int row)
{
  beginRemoveRows(QModelIndex(), row, row);

  m_DetectorProxies.remove(row);

  endRemoveRows();
}

void QxrdDetectorProxyListModel::moveDetectorDown(int row)
{
  int nRows = m_DetectorProxies.count();

  if (row >= 0 && row < (nRows-1)) {
    beginMoveRows(QModelIndex(), row+1, row+1, QModelIndex(), row);

    QxrdDetectorProxyPtr p1 = m_DetectorProxies[row];
    QxrdDetectorProxyPtr p2 = m_DetectorProxies[row+1];

    m_DetectorProxies[row] = p2;
    m_DetectorProxies[row+1] = p1;

    endMoveRows();
  }
}

void QxrdDetectorProxyListModel::moveDetectorUp(int row)
{
  int nRows = m_DetectorProxies.count();

  if (row >= 1 && row < nRows) {
    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row-1);

    QxrdDetectorProxyPtr p1 = m_DetectorProxies[row-1];
    QxrdDetectorProxyPtr p2 = m_DetectorProxies[row];

    m_DetectorProxies[row-1] = p2;
    m_DetectorProxies[row] = p1;

    endMoveRows();
  }
}

void QxrdDetectorProxyListModel::configureDetector(int row)
{
  QxrdDetectorProxyPtr p = m_DetectorProxies.value(row);

  if (p) {
    p->configureDetector();
  }
}
