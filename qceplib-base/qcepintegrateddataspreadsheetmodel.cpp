#include "qcepintegrateddataspreadsheetmodel.h"
#include "qcepintegrateddata.h"

QcepIntegratedDataSpreadsheetModel::QcepIntegratedDataSpreadsheetModel
  (QcepIntegratedDataWPtr integ)
  : m_Integrated(integ)
{

}

int QcepIntegratedDataSpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  QcepIntegratedDataPtr integ(m_Integrated);

  if (integ) {
    return integ->size();
  } else {
    return 0;
  }
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
      QcepIntegratedDataPtr integ(m_Integrated);

      if (integ) {
        if (index.column() == 0) {
          res = integ->x(index.row());
        } else if (index.column() == 1) {
          res = integ->y(index.row());
        }
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
