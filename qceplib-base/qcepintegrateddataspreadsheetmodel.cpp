#include "qcepintegrateddataspreadsheetmodel.h"
#include "qcepintegrateddata.h"

QcepIntegratedDataSpreadsheetModel::QcepIntegratedDataSpreadsheetModel
  (QcepIntegratedDataPtr integ)
  : m_Integrated(integ)
{

}

int QcepIntegratedDataSpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  return m_Integrated->size();
}

int QcepIntegratedDataSpreadsheetModel::columnCount(const QModelIndex &parent) const
{
  return 2;
}

QVariant QcepIntegratedDataSpreadsheetModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (index.isValid()) {
      if (index.column() == 0) {
        res = m_Integrated->x(index.row());
      } else if (index.column() == 1) {
        res = m_Integrated->y(index.row());
      }
    }
  }

  return res;
}

QVariant QcepIntegratedDataSpreadsheetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant res = QVariant();

  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      if (section == 0) {
        res = "x";
      } else if (section == 1) {
        res = "y";
      }
    } else {
      res = section;
    }
  }

  return res;
}
