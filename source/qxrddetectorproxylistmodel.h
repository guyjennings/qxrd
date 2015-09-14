#ifndef QXRDDETECTORPROXYLISTMODEL_H
#define QXRDDETECTORPROXYLISTMODEL_H

#include <QAbstractListModel>
#include "qxrddetectorproxy-ptr.h"

class QxrdDetectorProxyListModel : public QAbstractListModel
{
public:
  QxrdDetectorProxyListModel();

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;

  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;

  bool setData(const QModelIndex &index, const QVariant &value, int role);

  void append(QxrdDetectorProxyPtr proxy);
  void removeDetector(int row);

  void moveDetectorDown(int row);
  void moveDetectorUp(int row);

  void configureDetector(int row);

  QxrdDetectorProxyPtr detectorProxy(int i);

private:
  QVector<QxrdDetectorProxyPtr> m_DetectorProxies;
};

#endif // QXRDDETECTORPROXYLISTMODEL_H