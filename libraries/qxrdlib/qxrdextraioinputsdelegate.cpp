#include "qxrdextraioinputsdelegate.h"

QxrdExtraIOInputsDelegate::QxrdExtraIOInputsDelegate()
{

}

QWidget* QxrdExtraIOInputsDelegate::createEditor(QWidget *parent,
                                                  const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const
{
  return inherited::createEditor(parent, option, index);
}

void QxrdExtraIOInputsDelegate::setEditorData(QWidget *editor,
                                               const QModelIndex &index) const
{
  inherited::setEditorData(editor, index);
}

void QxrdExtraIOInputsDelegate::setModelData(QWidget *editor,
                                              QAbstractItemModel *model,
                                              const QModelIndex &index) const
{
  inherited::setModelData(editor, model, index);
}
