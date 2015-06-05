#ifndef QCEPDATACOLUMNSCAN_H
#define QCEPDATACOLUMNSCAN_H

#include <QObject>
#include "qcepdatagroup.h"

class QcepDataColumnScan : public QcepDataGroup
{
  Q_OBJECT

public:
  QcepDataColumnScan(QcepSettingsSaverWPtr sav, QString name, QStringList cols, int npts, QObject *parent = 0);
};

#endif // QCEPDATACOLUMNSCAN_H
