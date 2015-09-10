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
  beginInsertRows(QModelIndex(), m_DetectorProxies.count(), 1);

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
