#ifndef QXRDEXTRAIOINPUTSDELEGATE_H
#define QXRDEXTRAIOINPUTSDELEGATE_H

#include "qxrdlib_global.h"
#include <QStyledItemDelegate>
#include "qxrdsynchronizedacquisition-ptr.h"

class QXRD_EXPORT QxrdExtraIOInputsDelegate : public QStyledItemDelegate
{
  Q_OBJECT

private:
  typedef QStyledItemDelegate inherited;

public:
  QxrdExtraIOInputsDelegate(QxrdSynchronizedAcquisitionWPtr sync);

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

#endif // QXRDEXTRAIOINPUTSDELEGATE_H
