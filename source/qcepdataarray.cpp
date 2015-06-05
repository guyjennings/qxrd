#include "qcepdataarray.h"

QcepDataArray::QcepDataArray(QcepSettingsSaverWPtr saver, QString name, QVector<int> dims, QObject *parent) :
  QcepDataObject(saver, name, parent),
  m_Dimensions(dims)
{
  set_Type("array");

  int prod = 1;

  foreach (int d, dims) {
    prod *= d;
  }

  m_Data.resize(prod);
}

QVector<int> QcepDataArray::dimensions()
{
  return m_Dimensions;
}

QVector<double> QcepDataArray::vectorData()
{
  return m_Data;
}
