#include "qcepdatagroupspreadsheetmodel.h"
#include "qcepdatagroup.h"

QcepDataGroupSpreadsheetModel::QcepDataGroupSpreadsheetModel(QcepDataGroupWPtr group) :
  m_Group(group)
{

}

int QcepDataGroupSpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  QcepDataGroupPtr grp(m_Group);

  if (grp) {
    return grp->childCount();
  } else {
    return 0;
  }
}

int QcepDataGroupSpreadsheetModel::columnCount(const QModelIndex &parent) const
{
  return 3;
}

QVariant QcepDataGroupSpreadsheetModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (index.isValid()) {
      QcepDataGroupPtr grp(m_Group);

      if (grp) {
        QcepDataObjectPtr obj = grp->item(index.row());

        if (obj) {
          res = obj->columnData(index.column());
        }
      }
    }
  }

  return res;
}

QVariant QcepDataGroupSpreadsheetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      switch (section) {
      case 0:
        res = "Name";
        break;

      case 1:
        res = "Type";
        break;

      case 2:
        res = "Description";
        break;
      }
    } else if (orientation == Qt::Vertical) {
      res = section;
    }
  }

  return res;
}
