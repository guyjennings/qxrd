#include "qxrdextraiodetectorsdelegate.h"
#include "qxrdextraiodetectorsmodel.h"
#include <QComboBox>
#include "qxrddetectorsettings.h"
#include <QApplication>

QxrdExtraIODetectorsDelegate::QxrdExtraIODetectorsDelegate()
{

}

QWidget* QxrdExtraIODetectorsDelegate::createEditor(QWidget *parent,
                                                  const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const
{
  QComboBox *editor = new QComboBox(parent);

  switch (index.column()) {
  case QxrdExtraIODetectorsModel::ChannelNumberColumn:
    for (int i=1; i<4; i++) {
      editor -> addItem(tr("ctr%1").arg(i));
    }
    break;

  case QxrdExtraIODetectorsModel::DetectorNumberColumn:
    for (int i=0; i<6; i++) {
      editor -> addItem(tr("det%1").arg(i));
    }
    break;

  case QxrdExtraIODetectorsModel::DetectorTypeColumn:
    for (int i=0; i<QxrdDetectorSettings::detectorTypeCount(); i++) {
      editor -> addItem(QxrdDetectorSettings::detectorTypeName(i));
    }
    break;
  }

  return editor; /*inherited::createEditor(parent, option, index);*/
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
