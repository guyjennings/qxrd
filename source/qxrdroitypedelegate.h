#ifndef QXRDROITYPEDELEGATE_H
#define QXRDROITYPEDELEGATE_H

#include <QStyledItemDelegate>

class QxrdROITypeDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  QxrdROITypeDelegate(QWidget *parent=0);

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

#endif // QXRDROITYPEDELEGATE_H
