#ifndef QCEPDATACOLUMN_H
#define QCEPDATACOLUMN_H

#include <QObject>
#include <QVector>
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepdatacolumn-ptr.h"

/*

  A named 1-Dimensional data vector

 */

class QcepDataColumn : public QcepDataObject, public QVector<double>
{
  Q_OBJECT

public:
  QcepDataColumn(QcepSettingsSaverWPtr saver,
                 QString name,
                 int npts);

  static QcepDataColumnPtr newDataColumn(QcepSettingsSaverWPtr saver,
                                         QString name,
                                         int npts);

private:
  int m_NPoints;
};

#endif // QCEPDATACOLUMN_H
