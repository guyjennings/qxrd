#ifndef QXRDEXTRAIOINPUTSMODEL_H
#define QXRDEXTRAIOINPUTSMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractTableModel>
#include "qxrdsynchronizedacquisition-ptr.h"

class QXRD_EXPORT QxrdExtraIOInputsModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QxrdExtraIOInputsModel(QxrdSynchronizedAcquisitionWPtr sync);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;

private:
  QxrdSynchronizedAcquisitionWPtr m_Sync;
};

#endif // QXRDEXTRAIOINPUTSMODEL_H
