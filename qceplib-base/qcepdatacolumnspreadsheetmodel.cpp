#include "qcepdatacolumnspreadsheetmodel.h"
#include "qcepdatacolumn.h"

QcepDataColumnSpreadsheetModel::QcepDataColumnSpreadsheetModel
  (QcepDataColumnPtr col)
  : m_Column(col)
{

}

int QcepDataColumnSpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  if (m_Column) {
    return m_Column->rowCount();
  } else {
    return 0;
  }
}

int QcepDataColumnSpreadsheetModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant QcepDataColumnSpreadsheetModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (index.isValid()) {
      if (index.column() == 0) {
        res = m_Column->value(index.row());
      }
    }
  }

  return res;
}

QVariant QcepDataColumnSpreadsheetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant res = QVariant();

  if (m_Column) {
    if (role == Qt::DisplayRole) {
      if (orientation == Qt::Horizontal) {
        if (section == 0) {
          res = m_Column->get_Name();
        }
      } else {
        res = section;
      }
    }
  }

  return res;
}
