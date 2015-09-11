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
  return 3;
}

QVariant QxrdDetectorProxyListModel::data(const QModelIndex &index, int role) const
{
  int row = index.row();
  int col = index.column();

  if (role == Qt::DisplayRole) {
    QxrdDetectorProxyPtr p = m_DetectorProxies.value(row);

    if (p) {
      if (col == 1) {
        return p->detectorTypeName();
      } else if (col == 2) {
        return p->get_Name();
      }
    }
  }

  if (role == Qt::CheckStateRole) {
    QxrdDetectorProxyPtr p = m_DetectorProxies.value(row);

    if (p) {
      if (col == 0) {
        if (p->enabled()) {
          return Qt::Checked;
        } else {
          return Qt::Unchecked;
        }
      }
    }
  }

  return QVariant();
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
