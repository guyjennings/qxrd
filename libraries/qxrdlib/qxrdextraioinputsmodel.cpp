#include "qxrdextraioinputsmodel.h"

QxrdExtraIOInputsModel::QxrdExtraIOInputsModel(QxrdSynchronizedAcquisitionWPtr sync)
  : m_Sync(sync)
{

}

int QxrdExtraIOInputsModel::rowCount(const QModelIndex &parent) const
{
  return 5;
}

int QxrdExtraIOInputsModel::columnCount(const QModelIndex &parent) const
{
  return 4;
}

QVariant QxrdExtraIOInputsModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    return "Output Data";
  }

  return QVariant();
}
