#include "qcepdatagroupspreadsheetmodel.h"
#include "qcepdatagroup.h"

QcepDataGroupSpreadsheetModel::QcepDataGroupSpreadsheetModel(QcepDataGroupPtr group) :
  m_Group(group)
{

}

int QcepDataGroupSpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  return m_Group->childCount();
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
      QcepDataObjectPtr obj = m_Group->item(index.row());

      if (obj) {
        res = obj->columnData(index.column());
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
