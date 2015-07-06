#include "qcepdataarrayspreadsheetmodel.h"
#include "qcepdataarray.h"

QcepDataArraySpreadsheetModel::QcepDataArraySpreadsheetModel
  (QcepDataArrayPtr array)
  : m_Array(array)
{

}

int QcepDataArraySpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  QVector<int> dims = m_Array->dimensions();

  return dims.value(0);
}

int QcepDataArraySpreadsheetModel::columnCount(const QModelIndex &parent) const
{
  QVector<int> dims = m_Array->dimensions();

  return dims.value(1);
}

QVariant QcepDataArraySpreadsheetModel::data(const QModelIndex &index, int role) const
{
  return QVariant();
}
