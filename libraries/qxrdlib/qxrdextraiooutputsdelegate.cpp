#include "qxrdextraiooutputsdelegate.h"
#include "qxrdextraiooutputsmodel.h"
#include <QComboBox>
#include <QApplication>
#include <QDoubleSpinBox>
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizedoutputchannel-ptr.h"
#include "qxrdsynchronizedoutputchannel.h"

QxrdExtraIOOutputsDelegate::QxrdExtraIOOutputsDelegate(QxrdSynchronizedAcquisitionWPtr sync)
  : m_SynchronizedAcquisition(sync)
{

}

QWidget* QxrdExtraIOOutputsDelegate::createEditor(QWidget *parent,
                                                  const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const
{
  QWidget* res = NULL;
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  if (sync) {
    QxrdSynchronizedOutputChannelPtr out(sync->output(index.row()));

    if (out) {
      switch (index.column()) {
      case QxrdExtraIOOutputsModel::ChannelNumberColumn:
        {
          QComboBox *cb = new QComboBox(parent);

          for (int i=0; i<sync->outputDeviceCount(); i++) {
            cb -> addItem(sync->outputDeviceName(i));
          }

          res = cb;
        }
        break;

      case QxrdExtraIOOutputsModel::ChannelModeColumn:
        {
          QComboBox *cb = new QComboBox(parent);

          for (int i=0; i<out->channelModeCount(); i++) {
            cb -> addItem(out->channelMode(i));
          }

          res = cb;
        }
        break;

      case QxrdExtraIOOutputsModel::WaveformColumn:
        {
          QComboBox *cb = new QComboBox(parent);

          for (int i=0; i<out->waveformModeCount(); i++) {
            cb -> addItem(out->waveformMode(i));
          }

          res = cb;
        }
        break;

      case QxrdExtraIOOutputsModel::StartVColumn:
      case QxrdExtraIOOutputsModel::EndVColumn:
        {
          QDoubleSpinBox *sb = new QDoubleSpinBox(parent);

          sb -> setValue(index.data().toDouble());
          sb -> setMinimum(-20);
          sb -> setMaximum(20);
          sb -> setSingleStep(0.1);

          res = sb;
        }
        break;

      case QxrdExtraIOOutputsModel::SymmetryColumn:
        {
          QDoubleSpinBox *sb = new QDoubleSpinBox(parent);

          sb -> setValue(index.data().toDouble());
          sb -> setMinimum(-1);
          sb -> setMaximum(1);
          sb -> setSingleStep(0.01);

          res = sb;
        }
        break;

      case QxrdExtraIOOutputsModel::PhaseShiftColumn:
        {
          QDoubleSpinBox *sb = new QDoubleSpinBox(parent);

          sb -> setValue(index.data().toDouble());
          sb -> setMinimum(-450);
          sb -> setMaximum(450);

          res = sb;
        }
        break;
      }
    }
  }

  if (res == NULL) {
    res = inherited::createEditor(parent, option, index);
  }

  return res;
}

void QxrdExtraIOOutputsDelegate::setEditorData(QWidget *editor,
                                               const QModelIndex &index) const
{
  QComboBox* cb = qobject_cast<QComboBox*>(editor);

  if (cb) {
    int n = index.data(Qt::EditRole).toInt();

    cb -> setCurrentIndex(n);
  }

  QDoubleSpinBox* sb = qobject_cast<QDoubleSpinBox*>(editor);

  if (sb) {
    double v = index.data().toDouble();

    sb->setValue(v);
  }

  if (cb == NULL && sb == NULL) {
    inherited::setEditorData(editor, index);
  }
}

void QxrdExtraIOOutputsDelegate::setModelData(QWidget *editor,
                                              QAbstractItemModel *model,
                                              const QModelIndex &index) const
{
  QComboBox*      cb = qobject_cast<QComboBox*>(editor);
  QDoubleSpinBox* sb = qobject_cast<QDoubleSpinBox*>(editor);

  if (cb) {
    int n = cb->currentIndex();
    model->setData(index, n, Qt::EditRole);
  } else if (sb) {
    double v = sb->value();
    model->setData(index, v, Qt::EditRole);
  } else {
    inherited::setModelData(editor, model, index);
  }
}