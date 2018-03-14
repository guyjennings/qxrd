#ifndef QXRDEXTRAIOOUTPUTSDELEGATE_H
#define QXRDEXTRAIOOUTPUTSDELEGATE_H

#include "qxrdlib_global.h"
#include <QStyledItemDelegate>
#include "qxrdsynchronizedacquisition-ptr.h"

class QXRD_EXPORT QxrdExtraIOOutputsDelegate : public QStyledItemDelegate
{
  Q_OBJECT

private:
  typedef QStyledItemDelegate inherited;

public:
  QxrdExtraIOOutputsDelegate(QxrdSynchronizedAcquisitionWPtr sync);

  QWidget *createEditor(QWidget *parent,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;

  void setEditorData   (QWidget *editor,
                        const QModelIndex &index) const;

  void setModelData    (QWidget *editor,
                        QAbstractItemModel *model,
                        const QModelIndex &index) const;
private:
  QxrdSynchronizedAcquisitionWPtr m_SynchronizedAcquisition;
};

#endif // QXRDEXTRAIOOUTPUTSDELEGATE_H
