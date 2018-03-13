#include "qxrdextraiodetectorsmodel.h"

QxrdExtraIODetectorsModel::QxrdExtraIODetectorsModel(QxrdSynchronizedAcquisitionWPtr sync)
  : m_Sync(sync)
{

}

int QxrdExtraIODetectorsModel::rowCount(const QModelIndex &parent) const
{
  return 5;
}

int QxrdExtraIODetectorsModel::columnCount(const QModelIndex &parent) const
{
  return 4;
}

QVariant QxrdExtraIODetectorsModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    return "Detector Data";
  }

  return QVariant();
}
