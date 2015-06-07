#include "qcepdataarray.h"

QcepDataArray::QcepDataArray(QcepSettingsSaverWPtr saver, QString name, QVector<int> dims, QcepDataObjectWPtr parent) :
  QcepDataObject(saver, name, parent),
  m_Dimensions(dims)
{
  set_Type("Data Array");

  int prod = 1;

  QString desc = "[";

  for (int i=0; i<dims.count(); i++) {
    prod *= dims[i];

    if (i!=0) {
      desc += ", ";
    }

    desc += tr("%1").arg(dims[i]);
  }

  desc += "]";

  set_Description(desc);

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
