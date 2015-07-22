#include "qcepdatacolumnspreadsheetmodel.h"
#include "qcepdatacolumn.h"

QcepDataColumnSpreadsheetModel::QcepDataColumnSpreadsheetModel
  (QcepDataColumnWPtr col)
  : m_Column(col)
{

}

int QcepDataColumnSpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  QcepDataColumnPtr col(m_Column);

  if (col) {
    return col->rowCount();
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
        QcepDataColumnPtr col(m_Column);

        if (col) {
          res = col->value(index.row());
        }
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
          QcepDataColumnPtr col(m_Column);

          if (col) {
            res = col->get_Name();
          }
        }
      } else {
        res = section;
      }
    }
  }

  return res;
}
