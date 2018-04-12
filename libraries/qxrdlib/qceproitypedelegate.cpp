#include "qceproitypedelegate.h"
#include "qceproimodel.h"
#include <QComboBox>
#include "qceproi.h"
#include <QMenu>
#include "qceproishape.h"

QcepROITypeDelegate::QcepROITypeDelegate(QWidget *parent)
  : QStyledItemDelegate(parent)
{

}

QWidget *QcepROITypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
  if (index.column() == QcepROIModel::OuterTypeCol ||
      index.column() == QcepROIModel::InnerTypeCol) {
    QComboBox *editor = new QComboBox(parent);

    for (int i=0; i<QcepROIShape::roiTypeCount(); i++) {
      QString name = QcepROIShape::roiTypeName(i);
      editor->addItem(name);
    }

    return editor;
  } else {
    return QStyledItemDelegate::createEditor(parent, option, index);
  }
}

void QcepROITypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  if (index.column() == QcepROIModel::OuterTypeCol ||
      index.column() == QcepROIModel::InnerTypeCol) {
    QComboBox *cb = qobject_cast<QComboBox*>(editor);

    if (cb) {
      QString roiTypeName = index.data().toString();

      int cbindex = cb->findText(roiTypeName);

      if (cbindex >= 0) {
        cb->setCurrentIndex(cbindex);
      }

      connect(cb, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged,
              this, &QcepROITypeDelegate::typeChanged);
    }
  } else {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

void QcepROITypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
  if (index.column() == QcepROIModel::InnerTypeCol ||
      index.column() == QcepROIModel::OuterTypeCol) {
    QComboBox *cb = qobject_cast<QComboBox*>(editor);

    if (cb) {
      QString roiType = cb->currentText();

      model->setData(index, roiType, Qt::EditRole);
    }
  } else {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}

void QcepROITypeDelegate::typeChanged(int /*newType*/)
{
  QComboBox *cb = qobject_cast<QComboBox*>(sender());

  if (cb) {
    emit commitData(cb);
    emit closeEditor(cb);
  }
}
