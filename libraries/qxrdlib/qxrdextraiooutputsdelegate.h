#ifndef QXRDEXTRAIOOUTPUTSDELEGATE_H
#define QXRDEXTRAIOOUTPUTSDELEGATE_H

#include "qxrdlib_global.h"
#include <QStyledItemDelegate>

class QXRD_EXPORT QxrdExtraIOOutputsDelegate : public QStyledItemDelegate
{
  Q_OBJECT

private:
  typedef QStyledItemDelegate inherited;

public:
  QxrdExtraIOOutputsDelegate();

  QWidget *createEditor(QWidget *parent,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;

  void setEditorData   (QWidget *editor,
                        const QModelIndex &index) const;

  void setModelData    (QWidget *editor,
                        QAbstractItemModel *model,
                        const QModelIndex &index) const;
};

#endif // QXRDEXTRAIOOUTPUTSDELEGATE_H
