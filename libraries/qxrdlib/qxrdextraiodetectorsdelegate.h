#ifndef QXRDEXTRAIODETECTORSDELEGATE_H
#define QXRDEXTRAIODETECTORSDELEGATE_H

#include "qxrdlib_global.h"
#include <QStyledItemDelegate>

class QXRD_EXPORT QxrdExtraIODetectorsDelegate : public QStyledItemDelegate
{
  Q_OBJECT

private:
  typedef QStyledItemDelegate inherited;

public:
  QxrdExtraIODetectorsDelegate();

  QWidget *createEditor(QWidget *parent,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;

  void setEditorData   (QWidget *editor,
                        const QModelIndex &index) const;

  void setModelData    (QWidget *editor,
                        QAbstractItemModel *model,
                        const QModelIndex &index) const;

//  void paint           (QPainter *painter,
//                        const QStyleOptionViewItem &option,
//                        const QModelIndex &index) const;
};

#endif // QXRDEXTRAIODETECTORSDELEGATE_H
