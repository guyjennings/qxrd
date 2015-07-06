#include "qcepdatacolumnscanspreadsheetmodel.h"
#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"

QcepDataColumnScanSpreadsheetModel::QcepDataColumnScanSpreadsheetModel
  (QcepDataColumnScanPtr scan) :
  m_Scan(scan)
{

}

int QcepDataColumnScanSpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  return m_Scan->scanRowCount();
}

int QcepDataColumnScanSpreadsheetModel::columnCount(const QModelIndex &parent) const
{
  return m_Scan->scanColumnCount();
}

QVariant QcepDataColumnScanSpreadsheetModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (index.isValid()) {
      QcepDataColumnPtr col = m_Scan->column(index.column());

      if (col) {
        return col->value(index.row());
      }
    }
  }

  return res;
}

QVariant QcepDataColumnScanSpreadsheetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (orientation==Qt::Horizontal) {
      QcepDataColumnPtr col = m_Scan->column(section);

      if (col) {
        res = col->get_Name();
      }
    } else {
      res = section;
    }
  }

  return res;
}
