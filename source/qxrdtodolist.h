#ifndef QXRDTODOLIST_H
#define QXRDTODOLIST_H

#include <QAbstractListModel>
#include "qxrdtodolistitem-ptr.h"
#include "qcepobject-ptr.h"

class QxrdToDoList : public QAbstractListModel
{
  Q_OBJECT

public:
  QxrdToDoList(QObject *parent);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;

  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  typedef enum {
    IdentifierColumn,
    ActiveColumn,
    InsertedDateColumn,
    CompletedDateColumn,
    DescriptionColumn
  } Columns;

signals:

public slots:

private:
  QVector<QxrdToDoListItemPtr> m_Items;
};

#endif // QXRDTODOLIST_H
