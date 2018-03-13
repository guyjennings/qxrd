#include "qxrdextraiodetectorsmodel.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizeddetectorchannel.h"

QxrdExtraIODetectorsModel::QxrdExtraIODetectorsModel(QxrdSynchronizedAcquisitionWPtr sync)
  : m_Sync(sync)
{

}

int QxrdExtraIODetectorsModel::rowCount(const QModelIndex &parent) const
{
  int res = 0;

  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    res = sync->detectorCount();
  }

  return res;
}

int QxrdExtraIODetectorsModel::columnCount(const QModelIndex &parent) const
{
  return ColumnCount;
}

QVariant QxrdExtraIODetectorsModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync && index.isValid()) {
    int row = index.row();
    int col = index.column();

    QxrdSynchronizedDetectorChannelPtr det(sync->detector(row));

    if (det) {
      if (role == Qt::DisplayRole) {
        switch (col) {
        case ChannelNumberColumn:
          res = det->get_ChannelNumber();
          break;

        case DetectorNumberColumn:
          res = det->get_DetectorNumber();
          break;

        case DetectorTypeColumn:
          res = det->get_DetectorType();
          break;
        }
      }
    }
  }

  return res;
}

QVariant QxrdExtraIODetectorsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      switch (section) {
      case ChannelNumberColumn:
        res = "NIDAQ Channel";
        break;

      case DetectorNumberColumn:
        res = "Detector Number";
        break;

      case DetectorTypeColumn:
        res = "Detector Type";
        break;
      }
    } else {
      res = tr("Detector-%1").arg(section);
    }
  }

  return res;
}

Qt::ItemFlags QxrdExtraIODetectorsModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags res = inherited::flags(index);

  res |= Qt::ItemIsEditable;

  return res;
}

bool QxrdExtraIODetectorsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  return false;
}

void QxrdExtraIODetectorsModel::newDetector(int before)
{
  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    beginInsertRows(QModelIndex(), before, before);

    sync->newDetector(before);

    endInsertRows();
  }
}

void QxrdExtraIODetectorsModel::deleteDetector(int n)
{
  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    beginRemoveRows(QModelIndex(), n, n);

    sync->deleteDetector(n);

    endRemoveRows();
  }
}

void QxrdExtraIODetectorsModel::deleteDetectors(QVector<int> n)
{
  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    if (n.count() == 1) {
      deleteDetector(n.first());
    } else {
      beginResetModel();

      for (int i=n.count()-1; i>=0; i--) {
        sync->deleteDetector(n.value(i));
      }

      endResetModel();
    }
  }
}
