#include "qcepdatacolumnscanspreadsheetmodel.h"
#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"

QcepDataColumnScanSpreadsheetModel::QcepDataColumnScanSpreadsheetModel
  (QcepDataColumnScanWPtr scan) :
  m_Scan(scan)
{

}

int QcepDataColumnScanSpreadsheetModel::rowCount(const QModelIndex & /*parent*/) const
{
  QcepDataColumnScanPtr scan(m_Scan);

  if (scan) {
    return scan->rowCount();
  } else {
    return 0;
  }
}

int QcepDataColumnScanSpreadsheetModel::columnCount(const QModelIndex & /*parent*/) const
{
  QcepDataColumnScanPtr scan(m_Scan);

  if (scan) {
    return scan->columnCount();
  } else {
    return 0;
  }
}

QVariant QcepDataColumnScanSpreadsheetModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (index.isValid()) {
      QcepDataColumnScanPtr scan(m_Scan);

      if (scan) {
        QcepDataColumnPtr col = scan->column(index.column());

        if (col) {
          QcepDataColumnFormatter f = col->formatter();

          if (f) {
            res = f(col->value(index.row()));
          } else {
            res = col->value(index.row());
          }
        }
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
      QcepDataColumnScanPtr scan(m_Scan);

      if (scan) {
        QcepDataColumnPtr col = scan->column(section);

        if (col) {
          res = col->get_Name();
        }
      }
    } else {
      res = section;
    }
  }

  return res;
}
