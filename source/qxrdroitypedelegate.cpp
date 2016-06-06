#include "qxrdroitypedelegate.h"
#include "qxrdroicoordinateslistmodel.h"
#include <QComboBox>
#include "qxrdroicoordinates.h"
#include <QMenu>
#include "qxrdroishape.h"

QxrdROITypeDelegate::QxrdROITypeDelegate(QWidget *parent)
  : QStyledItemDelegate(parent)
{

}

QWidget *QxrdROITypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
  if (index.column() == QxrdROICoordinatesListModel::OuterTypeCol ||
      index.column() == QxrdROICoordinatesListModel::InnerTypeCol) {
    QComboBox *editor = new QComboBox(parent);

    for (int i=0; i<QxrdROIShape::roiTypeCount(); i++) {
      QString name = QxrdROIShape::roiTypeName(i);
      editor->addItem(name);
    }

    return editor;
  } else {
    return QStyledItemDelegate::createEditor(parent, option, index);
  }
}

void QxrdROITypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  if (index.column() == QxrdROICoordinatesListModel::OuterTypeCol ||
      index.column() == QxrdROICoordinatesListModel::InnerTypeCol) {
    QComboBox *cb = qobject_cast<QComboBox*>(editor);

    if (cb) {
      QString roiTypeName = index.data().toString();

      int cbindex = cb->findText(roiTypeName);

      if (cbindex >= 0) {
        cb->setCurrentIndex(cbindex);
      }

      connect(cb, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged,
              this, &QxrdROITypeDelegate::typeChanged);
    }
  } else {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

void QxrdROITypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
  if (index.column() == QxrdROICoordinatesListModel::InnerTypeCol ||
      index.column() == QxrdROICoordinatesListModel::OuterTypeCol) {
    QComboBox *cb = qobject_cast<QComboBox*>(editor);

    if (cb) {
      QString roiType = cb->currentText();

      model->setData(index, roiType, Qt::EditRole);
    }
  } else {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}

void QxrdROITypeDelegate::typeChanged(int newType)
{
  QComboBox *cb = qobject_cast<QComboBox*>(sender());

  if (cb) {
    emit commitData(cb);
    emit closeEditor(cb);
  }
}
