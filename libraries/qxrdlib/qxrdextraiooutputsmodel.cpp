#include "qxrdextraiooutputsmodel.h"

QxrdExtraIOOutputsModel::QxrdExtraIOOutputsModel(QxrdSynchronizedAcquisitionWPtr sync)
  : m_Sync(sync)
{

}

int QxrdExtraIOOutputsModel::rowCount(const QModelIndex &parent) const
{
  return 5;
}

int QxrdExtraIOOutputsModel::columnCount(const QModelIndex &parent) const
{
  return 4;
}

QVariant QxrdExtraIOOutputsModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    return "Output Data";
  }

  return QVariant();
}
