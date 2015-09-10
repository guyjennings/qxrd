#ifndef QXRDDETECTORPROXYLISTMODEL_H
#define QXRDDETECTORPROXYLISTMODEL_H

#include <QAbstractListModel>
#include "qxrddetectorproxy-ptr.h"

class QxrdDetectorProxyListModel : public QAbstractListModel
{
public:
  QxrdDetectorProxyListModel();

  int rowCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;

  void append(QxrdDetectorProxyPtr proxy);
  void removeDetector(int row);

  QxrdDetectorProxyPtr detectorProxy(int i);

private:
  QVector<QxrdDetectorProxyPtr> m_DetectorProxies;
};

#endif // QXRDDETECTORPROXYLISTMODEL_H
