#ifndef QXRDEXTRAIODETECTORSMODEL_H
#define QXRDEXTRAIODETECTORSMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractTableModel>
#include "qxrdsynchronizedacquisition-ptr.h"

class QXRD_EXPORT QxrdExtraIODetectorsModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QxrdExtraIODetectorsModel(QxrdSynchronizedAcquisitionWPtr sync);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;

private:
  QxrdSynchronizedAcquisitionWPtr m_Sync;
};

#endif // QXRDEXTRAIODETECTORSMODEL_H
