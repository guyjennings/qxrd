#include "qxrdextraiodetectorsdelegate.h"

QxrdExtraIODetectorsDelegate::QxrdExtraIODetectorsDelegate()
{

}

QWidget* QxrdExtraIODetectorsDelegate::createEditor(QWidget *parent,
                                                  const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const
{
  return inherited::createEditor(parent, option, index);
}

void QxrdExtraIODetectorsDelegate::setEditorData(QWidget *editor,
                                               const QModelIndex &index) const
{
  inherited::setEditorData(editor, index);
}

void QxrdExtraIODetectorsDelegate::setModelData(QWidget *editor,
                                              QAbstractItemModel *model,
                                              const QModelIndex &index) const
{
  inherited::setModelData(editor, model, index);
}
