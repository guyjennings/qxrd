#ifndef QXRDEXTRAIODETECTORSMODEL_H
#define QXRDEXTRAIODETECTORSMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractTableModel>
#include "qxrdsynchronizedacquisition-ptr.h"

class QXRD_EXPORT QxrdExtraIODetectorsModel : public QAbstractTableModel
{
  Q_OBJECT

private:
  typedef QAbstractTableModel inherited;

public:
  QxrdExtraIODetectorsModel(QxrdSynchronizedAcquisitionWPtr sync);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);

  enum {
    ChannelNumberColumn,
    DetectorNumberColumn,
    DetectorTypeColumn,
    ColumnCount
  };

  void newDetector(int before);
  void deleteDetector(int n);
  void deleteDetectors(QVector<int> n);

private:
  QxrdSynchronizedAcquisitionWPtr m_SynchronizedAcquisition;
};

#endif // QXRDEXTRAIODETECTORSMODEL_H
