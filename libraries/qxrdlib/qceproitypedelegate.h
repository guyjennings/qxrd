#ifndef QCEPROITYPEDELEGATE_H
#define QCEPROITYPEDELEGATE_H

#include "qceplib_global.h"
#include <QStyledItemDelegate>

class QCEP_EXPORT QcepROITypeDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  QcepROITypeDelegate(QWidget *parent=0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const Q_DECL_OVERRIDE;
  void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const Q_DECL_OVERRIDE;
signals:

public slots:

private slots:
  void typeChanged(int newType);
};

#endif // QCEPROITYPEDELEGATE_H
