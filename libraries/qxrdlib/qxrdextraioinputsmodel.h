#ifndef QXRDEXTRAIOINPUTSMODEL_H
#define QXRDEXTRAIOINPUTSMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractTableModel>
#include "qxrdsynchronizedacquisition-ptr.h"

class QXRD_EXPORT QxrdExtraIOInputsModel : public QAbstractTableModel
{
  Q_OBJECT

private:
  typedef QAbstractTableModel inherited;

public:
  QxrdExtraIOInputsModel(QxrdSynchronizedAcquisitionWPtr sync);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);

  enum {
    ChannelNumberColumn,
    SignalNameColumn,
    ChannelModeColumn,
    SaveWaveColumn,
    MinColumn,
    MaxColumn,
    ColumnCount
  };

  void newInput(int before);
  void deleteInput(int n);
  void deleteInputs(QVector<int> n);

private:
  QxrdSynchronizedAcquisitionWPtr m_Sync;
};

#endif // QXRDEXTRAIOINPUTSMODEL_H
