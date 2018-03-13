#ifndef QXRDEXTRAIOOUTPUTSMODEL_H
#define QXRDEXTRAIOOUTPUTSMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractTableModel>
#include "qxrdsynchronizedacquisition-ptr.h"

class QXRD_EXPORT QxrdExtraIOOutputsModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QxrdExtraIOOutputsModel(QxrdSynchronizedAcquisitionWPtr sync);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;

private:
  QxrdSynchronizedAcquisitionWPtr m_Sync;
};

#endif // QXRDEXTRAIOOUTPUTSMODEL_H
