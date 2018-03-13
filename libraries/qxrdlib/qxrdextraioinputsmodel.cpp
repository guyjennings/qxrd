#include "qxrdextraioinputsmodel.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizedinputchannel.h"

QxrdExtraIOInputsModel::QxrdExtraIOInputsModel(QxrdSynchronizedAcquisitionWPtr sync)
  : m_Sync(sync)
{

}

int QxrdExtraIOInputsModel::rowCount(const QModelIndex &parent) const
{
  int res = 0;

  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    res = sync->inputCount();
  }

  return res;
}

int QxrdExtraIOInputsModel::columnCount(const QModelIndex &parent) const
{
  return ColumnCount;
}

QVariant QxrdExtraIOInputsModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync && index.isValid()) {
    int row = index.row();
    int col = index.column();

    QxrdSynchronizedInputChannelPtr inp(sync->input(row));

    if (inp) {
      if (role == Qt::DisplayRole) {
        switch (col) {
        case ChannelNumberColumn:
          res = inp->get_ChannelNumber();
          break;

        case ChannelNameColumn:
          res = inp->get_ChannelName();
          break;

        case ChannelModeColumn:
          res = inp->get_Mode();
          break;

        case SaveWaveColumn:
          res = inp->get_SaveWave();
          break;

        case MinColumn:
          res = inp->get_Min();
          break;

        case MaxColumn:
          res = inp->get_Max();
          break;
        }
      }
    }
  }

  return res;
}

QVariant QxrdExtraIOInputsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      res = tr("Col-%1").arg(section);
      switch (section) {
      case ChannelNumberColumn:
        res = "NIDAQ Channel";
        break;

      case ChannelNameColumn:
        res = "Signal Name";
        break;

      case ChannelModeColumn:
        res = "Input Mode";
        break;

      case SaveWaveColumn:
        res = "Save Wfm?";
        break;

      case MinColumn:
        res = "Min V";
        break;

      case MaxColumn:
        res = "Max V";
        break;
      }
    } else {
      res = tr("Input-%1").arg(section);
    }
  }

  return res;
}

Qt::ItemFlags QxrdExtraIOInputsModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags res = inherited::flags(index);

  res |= Qt::ItemIsEditable;

  return res;
}

bool QxrdExtraIOInputsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  return false;
}

void QxrdExtraIOInputsModel::newInput(int before)
{
  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    beginInsertRows(QModelIndex(), before, before);

    sync->newInput(before);

    endInsertRows();
  }
}

void QxrdExtraIOInputsModel::deleteInput(int n)
{
  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    beginRemoveRows(QModelIndex(), n, n);

    sync->deleteInput(n);

    endRemoveRows();
  }
}

void QxrdExtraIOInputsModel::deleteInputs(QVector<int> n)
{
  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    if (n.count() == 1) {
      deleteInput(n.first());
    } else {
      beginResetModel();

      for (int i=n.count()-1; i>=0; i--) {
        sync->deleteInput(n.value(i));
      }

      endResetModel();
    }
  }
}
