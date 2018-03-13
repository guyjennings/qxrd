#include "qxrdextraiooutputsdelegate.h"

QxrdExtraIOOutputsDelegate::QxrdExtraIOOutputsDelegate()
{

}

QWidget* QxrdExtraIOOutputsDelegate::createEditor(QWidget *parent,
                                                  const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const
{
  return inherited::createEditor(parent, option, index);
}

void QxrdExtraIOOutputsDelegate::setEditorData(QWidget *editor,
                                               const QModelIndex &index) const
{
  inherited::setEditorData(editor, index);
}

void QxrdExtraIOOutputsDelegate::setModelData(QWidget *editor,
                                              QAbstractItemModel *model,
                                              const QModelIndex &index) const
{
  inherited::setModelData(editor, model, index);
}
