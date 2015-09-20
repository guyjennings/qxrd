#include "qxrdtodolist.h"
#include "qxrdtodolistitem.h"
#include "qcepmacros.h"

#define TODO(d1,d2,s) m_Items.append(QxrdToDoListItemPtr(new QxrdToDoListItem(true,d1,d2,s)));
#define DONE(d1,d2,s) m_Items.append(QxrdToDoListItemPtr(new QxrdToDoListItem(false,d1,d2,s)));

QxrdToDoList::QxrdToDoList()
{
#include "TODO.h"
}

int QxrdToDoList::rowCount(const QModelIndex &parent) const
{
  return m_Items.count();
}

int QxrdToDoList::columnCount(const QModelIndex &parent) const
{
  return 4;
}

QVariant QxrdToDoList::data(const QModelIndex &index, int role) const
{
  int row = index.row();
  int col = index.column();

  QxrdToDoListItemPtr item = m_Items.value(row);

  if (role == Qt::DisplayRole && item) {
    if (col == 0) {
      return item->isActive();
    } else if (col == 1) {
      return item->insertedDate();
    } else if (col == 2) {
      return item->completedDate();
    } else if (col == 3) {
      return item->description();
    }
  }

  return QVariant();
}

QVariant QxrdToDoList::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      if (section == 0) {
        return "Active";
      } else if (section == 1) {
        return "Started";
      } else if (section == 2) {
        return "Completed";
      } else if (section == 3) {
        return "Description";
      }
    }
  }

  return QVariant();
}
