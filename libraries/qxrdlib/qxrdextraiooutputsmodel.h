#ifndef QXRDEXTRAIOOUTPUTSMODEL_H
#define QXRDEXTRAIOOUTPUTSMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractTableModel>
#include "qxrdsynchronizedacquisition-ptr.h"

class QXRD_EXPORT QxrdExtraIOOutputsModel : public QAbstractTableModel
{
  Q_OBJECT

private:
  typedef QAbstractTableModel inherited;

public:
  QxrdExtraIOOutputsModel(QxrdSynchronizedAcquisitionWPtr sync);

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
    WaveformColumn,
    StartVColumn,
    EndVColumn,
    SymmetryColumn,
    PhaseShiftColumn,
    SampleRateColumn,
    ColumnCount
  };

  void newOutput(int before);
  void deleteOutput(int n);
  void deleteOutputs(QVector<int> n);

private:
  QxrdSynchronizedAcquisitionWPtr m_SynchronizedAcquisition;
};

#endif // QXRDEXTRAIOOUTPUTSMODEL_H
