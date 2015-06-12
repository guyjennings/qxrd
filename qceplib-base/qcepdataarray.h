#ifndef QCEPDATAARRAY_H
#define QCEPDATAARRAY_H

#include <QObject>
#include "qcepdataobject.h"
#include "qcepdataarray-ptr.h"
#include <QVector>

class QcepDataArray : public QcepDataObject
{
  Q_OBJECT

public:
  QcepDataArray(QcepSettingsSaverWPtr saver,
                QString name,
                QVector<int> dims);
  typedef QcepDataObject inherited;

  static QcepDataArrayPtr newDataArray(QcepSettingsSaverWPtr saver,
                                       QString name,
                                       QVector<int> dims);

public slots:
  QVector<int>    dimensions();
  QVector<double> vectorData();

private:
  QVector<int>    m_Dimensions;
  QVector<double> m_Data;
};

#endif // QCEPDATAARRAY_H
