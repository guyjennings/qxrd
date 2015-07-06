#include "qxrdintegrateddataspreadsheetmodel.h"
#include "qxrdintegrateddata.h"

QxrdIntegratedDataSpreadsheetModel::QxrdIntegratedDataSpreadsheetModel
  (QxrdIntegratedDataPtr integ)
  : m_Integrated(integ)
{

}

int QxrdIntegratedDataSpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  return m_Integrated->size();
}

int QxrdIntegratedDataSpreadsheetModel::columnCount(const QModelIndex &parent) const
{
  return 2;
}

QVariant QxrdIntegratedDataSpreadsheetModel::data(const QModelIndex &index, int role) const
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

QVariant QxrdIntegratedDataSpreadsheetModel::headerData(int section, Qt::Orientation orientation, int role) const
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
