#include "qxrddetectorproxylistmodel.h"
#include "qxrddetectorproxy.h"

QxrdDetectorProxyListModel::QxrdDetectorProxyListModel()
{
}

int QxrdDetectorProxyListModel::rowCount(const QModelIndex &parent) const
{
  return m_DetectorProxies.count();
}

QVariant QxrdDetectorProxyListModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    int row = index.row();

    QxrdDetectorProxyPtr p = m_DetectorProxies.value(row);

    if (p) {
      return p->detectorTypeName();
    }

    return "<<NULL>>";
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
}
