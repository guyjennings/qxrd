#ifndef QCEPDATAARRAY_H
#define QCEPDATAARRAY_H

#include <QObject>
#include "qcepdataobject.h"
#include <QVector>

class QcepDataArray : public QcepDataObject
{
  Q_OBJECT

public:
  QcepDataArray(QString name, QVector<int> dims, QObject *parent = 0);

private:
  QVector<int>    m_Dimensions;
  QVector<double> m_Data;
};

#endif // QCEPDATAARRAY_H
