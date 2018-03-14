#include "qxrdextraioinputsdelegate.h"
#include "qxrdextraioinputsmodel.h"
#include <QComboBox>
#include <QApplication>
#include <QDoubleSpinBox>
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizedinputchannel-ptr.h"
#include "qxrdsynchronizedinputchannel.h"

QxrdExtraIOInputsDelegate::QxrdExtraIOInputsDelegate(QxrdSynchronizedAcquisitionWPtr sync)
  : m_SynchronizedAcquisition(sync)
{
}

QWidget* QxrdExtraIOInputsDelegate::createEditor(QWidget *parent,
                                                  const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const
{
  QWidget* res = NULL;

  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  if (sync) {
    QxrdSynchronizedInputChannelPtr inp(sync->input(index.row()));

    if (inp) {
      switch (index.column()) {
      case QxrdExtraIOInputsModel::ChannelNumberColumn:
        {
          QComboBox *cb = new QComboBox(parent);

          for (int i=0; i<sync->inputDeviceCount(); i++) {
            cb -> addItem(sync->inputDeviceName(i));
          }

          res = cb;
        }
        break;

      case QxrdExtraIOInputsModel::ChannelModeColumn:
        {
          QComboBox *cb = new QComboBox(parent);

          for (int i=0; i<inp->channelModeCount(); i++) {
            cb -> addItem(inp->channelMode(i));
          }

          res = cb;
        }
        break;

      case QxrdExtraIOInputsModel::MinColumn:
      case QxrdExtraIOInputsModel::MaxColumn:
        {
          QDoubleSpinBox *sb = new QDoubleSpinBox(parent);

          sb -> setValue(index.data().toDouble());
          sb -> setMinimum(-20);
          sb -> setMaximum(20);

          res = sb;
        }
        break;

      case QxrdExtraIOInputsModel::SampleRateColumn:
        {
          QDoubleSpinBox *sb = new QDoubleSpinBox(parent);

          sb -> setValue(index.data().toDouble());
          sb -> setMinimum(0);
          sb -> setMaximum(50000);

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

void QxrdExtraIOInputsDelegate::setEditorData(QWidget *editor,
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

void QxrdExtraIOInputsDelegate::setModelData(QWidget *editor,
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
