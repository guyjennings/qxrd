#include "qxrdacquisitionscalermodel.h"

QxrdAcquisitionScalerModel::QxrdAcquisitionScalerModel(QxrdAcquisitionWPtr acq)
  : m_Acquisition(acq)
{

}

int QxrdAcquisitionScalerModel::rowCount(const QModelIndex &parent) const
{
  return 3;
}

int QxrdAcquisitionScalerModel::columnCount(const QModelIndex &parent) const
{
  return 4;
}

QVariant QxrdAcquisitionScalerModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    return "XXX";
  }

  return QVariant();
}

QVariant QxrdAcquisitionScalerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole) {
    return "HHH";
  }

  return QVariant();
}
