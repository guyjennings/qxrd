#include "qcepdataarray.h"

QcepDataArray::QcepDataArray(QString name, QVector<int> dims, QObject *parent) :
  m_Dimensions(dims),
  QcepDataObject(name, parent)
{

}

