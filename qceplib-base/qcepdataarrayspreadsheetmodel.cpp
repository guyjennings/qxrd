#include "qcepdataarrayspreadsheetmodel.h"
#include "qcepdataarray.h"

QcepDataArraySpreadsheetModel::QcepDataArraySpreadsheetModel
  (QcepDataArrayWPtr array)
  : m_Array(array)
{

}

int QcepDataArraySpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  QcepDataArrayPtr array(m_Array);

  if (array) {
    QVector<int> dims = array->dimensions();

    return dims.value(0);
  } else {
    return 0;
  }
}

int QcepDataArraySpreadsheetModel::columnCount(const QModelIndex &parent) const
{
  QcepDataArrayPtr array(m_Array);

  if (array) {
    QVector<int> dims = array->dimensions();

    return dims.value(1);
  } else {
    return 0;
  }
}

QVariant QcepDataArraySpreadsheetModel::data(const QModelIndex &index, int role) const
{
  return QVariant();
}
