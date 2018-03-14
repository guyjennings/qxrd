#include "qxrdextraiodetectorsdelegate.h"
#include "qxrdextraiodetectorsmodel.h"
#include <QComboBox>
#include "qxrddetectorsettings.h"
#include <QApplication>
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizeddetectorchannel-ptr.h"
#include "qxrdsynchronizeddetectorchannel.h"
#include "qxrdacqcommon.h"
#include "qxrdexperiment.h"

QxrdExtraIODetectorsDelegate::QxrdExtraIODetectorsDelegate(QxrdSynchronizedAcquisitionWPtr sync)
  : m_SynchronizedAcquisition(sync)
{

}

QWidget* QxrdExtraIODetectorsDelegate::createEditor(QWidget *parent,
                                                  const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const
{
  QWidget *res = NULL;

  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  QxrdExperimentPtr exp(QxrdExperiment::findExperiment(m_SynchronizedAcquisition));

  QxrdAcqCommonPtr  acq;

  if (exp) {
    acq = exp->acquisition();
  }

  if (sync && acq) {
    QxrdSynchronizedDetectorChannelPtr det(sync->detector(index.row()));

    if (det) {
      switch (index.column()) {
      case QxrdExtraIODetectorsModel::ChannelNumberColumn:
        {
          QComboBox *editor = new QComboBox(parent);

          for (int i=0; i<sync->detectorDeviceCount(); i++) {
            editor -> addItem(sync->detectorDeviceName(i));
          }

          res = editor;
        }
        break;

      case QxrdExtraIODetectorsModel::DetectorNumberColumn:
        {
          QComboBox *editor = new QComboBox(parent);

          for (int i=0; i<acq->detectorCount(); i++) {
            QxrdDetectorSettingsPtr det = acq->detector(i);

            if (det) {
              editor -> addItem(tr("%1 : %2").arg(i).arg(det->get_DetectorName()));
            }
          }

          res = editor;
        }
        break;

      case QxrdExtraIODetectorsModel::DetectorTypeColumn:
        {
          QComboBox *editor = new QComboBox(parent);

          for (int i=0; i<QxrdDetectorSettings::detectorTypeCount(); i++) {
            editor -> addItem(QxrdDetectorSettings::detectorTypeName(i));
          }

          res = editor;
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

void QxrdExtraIODetectorsDelegate::setEditorData(QWidget *editor,
                                               const QModelIndex &index) const
{
  int n = index.data(Qt::EditRole).toInt();
  QComboBox* cb = qobject_cast<QComboBox*>(editor);

  if (cb) {
    cb->setCurrentIndex(n);
  } else {
    inherited::setEditorData(editor, index);
  }
}

void QxrdExtraIODetectorsDelegate::setModelData(QWidget *editor,
                                              QAbstractItemModel *model,
                                              const QModelIndex &index) const
{
  QComboBox* cb = qobject_cast<QComboBox*>(editor);

  if (cb) {
    int n = cb->currentIndex();

    model->setData(index, n, Qt::EditRole);
  } else {
    inherited::setModelData(editor, model, index);
  }
}

//void QxrdExtraIODetectorsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
//{
//  QStyleOptionComboBox box;

//  box.rect = option.rect;
//  box.currentText = index.data().toString();

//  QApplication::style()->drawComplexControl(QStyle::CC_ComboBox, &box, painter, 0);
//  QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel, &box, painter, 0);
//}
