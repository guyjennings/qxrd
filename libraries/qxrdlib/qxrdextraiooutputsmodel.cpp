#include "qxrdextraiooutputsmodel.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizedoutputchannel.h"

QxrdExtraIOOutputsModel::QxrdExtraIOOutputsModel(QxrdSynchronizedAcquisitionWPtr sync)
  : m_Sync(sync)
{

}

int QxrdExtraIOOutputsModel::rowCount(const QModelIndex &parent) const
{
  int res = 0;

  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    res = sync->outputCount();
  }

  return res;
}

int QxrdExtraIOOutputsModel::columnCount(const QModelIndex &parent) const
{
  return ColumnCount;
}

QVariant QxrdExtraIOOutputsModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync && index.isValid()) {
    int row = index.row();
    int col = index.column();

    QxrdSynchronizedOutputChannelPtr out(sync->output(row));

    if (out) {
      if (role == Qt::DisplayRole) {
        switch (col) {
        case ChannelNumberColumn:
          res = out->channelName();
          break;

        case SignalNameColumn:
          res = out->get_SignalName();
          break;

        case ChannelModeColumn:
          res = out->channelMode();
          break;

        case StartVColumn:
          res = out->get_StartV();
          break;

        case EndVColumn:
          res = out->get_EndV();
          break;
        }
      } else if (role == Qt::EditRole) {
        switch (col) {
        case ChannelNumberColumn:
          res = out->get_ChannelNumber();
          break;

        case SignalNameColumn:
          res = out->get_SignalName();
          break;

        case ChannelModeColumn:
          res = out->get_Mode();
          break;

        case StartVColumn:
          res = out->get_StartV();
          break;

        case EndVColumn:
          res = out->get_EndV();
          break;
        }
      }
    }
  }

  return res;
}

QVariant QxrdExtraIOOutputsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      switch(section) {
      case ChannelNumberColumn:
        res = "NIDAQ Channel";
        break;

      case SignalNameColumn:
        res = "Signal Name";
        break;

      case ChannelModeColumn:
        res = "Mode";
        break;

      case StartVColumn:
        res = "Start V";
        break;

      case EndVColumn:
        res = "End V";
        break;
      }
    } else {
      res = tr("Output-%1").arg(section);
    }
  }

  return res;
}

Qt::ItemFlags QxrdExtraIOOutputsModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags res = inherited::flags(index);

  res |= Qt::ItemIsEditable;

  return res;
}

bool QxrdExtraIOOutputsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  bool res = false;

  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync && index.isValid()) {
    int row = index.row();
    int col = index.column();

    QxrdSynchronizedOutputChannelPtr out(sync->output(row));

    if (out) {
      if (role == Qt::EditRole) {
        switch (col) {
        case ChannelNumberColumn:
          out->set_ChannelNumber(value.toInt());
          res = true;
          break;

        case SignalNameColumn:
          out->set_SignalName(value.toString());
          res = true;
          break;

        case ChannelModeColumn:
          out->set_Mode(value.toInt());
          res = true;
          break;

        case StartVColumn:
          out->set_StartV(value.toDouble());
          res = true;
          break;

        case EndVColumn:
          out->set_EndV(value.toDouble());
          res = true;
          break;
        }
      }
    }
  }

  return res;
}

void QxrdExtraIOOutputsModel::newOutput(int before)
{
  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    beginInsertRows(QModelIndex(), before, before);

    sync->newOutput(before);

    endInsertRows();
  }
}

void QxrdExtraIOOutputsModel::deleteOutput(int n)
{
  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    beginRemoveRows(QModelIndex(), n, n);

    sync->deleteOutput(n);

    endRemoveRows();
  }
}

void QxrdExtraIOOutputsModel::deleteOutputs(QVector<int> n)
{
  QxrdSynchronizedAcquisitionPtr sync(m_Sync);

  if (sync) {
    if (n.count() == 1) {
      deleteOutput(n.first());
    } else {
      beginResetModel();

      for (int i=n.count()-1; i>=0; i--) {
        sync->deleteOutput(n.value(i));
      }

      endResetModel();
    }
  }
}
