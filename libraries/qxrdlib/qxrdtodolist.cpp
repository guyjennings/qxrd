#include "qxrdtodolist.h"
#include "qxrdtodolistitem.h"
#include "qcepmacros.h"
#include <QFont>

#define TODO(d1,d2,s) m_Items.append(QxrdToDoListItemPtr(NEWPTR(QxrdToDoListItem(parent,id++,true,d1,d2,s))));
#define DONE(d1,d2,s) m_Items.append(QxrdToDoListItemPtr(NEWPTR(QxrdToDoListItem(parent,id++,false,d1,d2,s))));

QxrdToDoList::QxrdToDoList(QObject *parent)
  : QAbstractListModel()
{
  int id=0;

#include "TODO.h"
}

int QxrdToDoList::rowCount(const QModelIndex & /*parent*/) const
{
  return m_Items.count();
}

int QxrdToDoList::columnCount(const QModelIndex & /*parent*/) const
{
  return 5;
}

QVariant QxrdToDoList::data(const QModelIndex &index, int role) const
{
  int row = index.row();
  int col = index.column();

  QxrdToDoListItemPtr item = m_Items.value(row);

  if (role == Qt::DisplayRole && item) {
    if (col == IdentifierColumn) {
      return item->identifier();
    } else if (col == ActiveColumn) {
      return item->isActive();
    } else if (col == InsertedDateColumn) {
      return item->insertedDate().date();
    } else if (col == CompletedDateColumn) {
      return item->completedDate().date();
    } else if (col == DescriptionColumn) {
      return item->description();
    }
  } else if (role == Qt::TextAlignmentRole) {
    if (col == IdentifierColumn || col == ActiveColumn || col == InsertedDateColumn || col == CompletedDateColumn) {
      return QVariant(Qt::AlignHCenter | Qt::AlignTop);
    } else if (col == DescriptionColumn) {
      return QVariant(Qt::AlignLeft | Qt::AlignTop);
    }
//  } else if (role == Qt::FontRole) {
//    QFont f;
//    f.setPointSize(10);

//    return QVariant(f);
  }

  return QVariant();
}

QVariant QxrdToDoList::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      if (section == IdentifierColumn) {
        return "Id";
      } else if (section == ActiveColumn) {
        return "Active";
      } else if (section == InsertedDateColumn) {
        return "Started";
      } else if (section == CompletedDateColumn) {
        return "Completed";
      } else if (section == DescriptionColumn) {
        return "Description";
      }
    }
  }

  return QVariant();
}
