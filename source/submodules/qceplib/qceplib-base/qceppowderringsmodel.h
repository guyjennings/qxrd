#ifndef QCEPPOWDERRINGSMODEL_H
#define QCEPPOWDERRINGSMODEL_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include <QAbstractTableModel>

class QCEP_EXPORT QcepPowderRingsModel : public QAbstractTableModel
{
public:
  QcepPowderRingsModel();

  // Basic functionality:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  bool setData(const QModelIndex &index, const QVariant &value, int role) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  enum {
    NumCol,
    XCol,
    YCol,
    ColCount
  };
};

#endif // QCEPPOWDERRINGSMODEL_H
