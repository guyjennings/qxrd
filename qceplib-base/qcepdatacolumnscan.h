#ifndef QCEPDATACOLUMNSCAN_H
#define QCEPDATACOLUMNSCAN_H

#include <QObject>
#include "qcepdatagroup.h"

class QcepDataColumnScan : public QcepDataGroup
{
  Q_OBJECT

public:
  QcepDataColumnScan(QcepSettingsSaverWPtr sav,
                     QString name,
                     QStringList cols,
                     int npts,
                     QcepDataObjectWPtr parent = QcepDataObjectWPtr());

  typedef QcepDataGroup inherited;

public:
  Q_PROPERTY(int numPoints READ get_NumPoints WRITE set_NumPoints)
  QCEP_INTEGER_PROPERTY(NumPoints)
};

#endif // QCEPDATACOLUMNSCAN_H
