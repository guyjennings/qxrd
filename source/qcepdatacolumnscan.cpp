#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"

QcepDataColumnScan::QcepDataColumnScan(QString name, QStringList cols, int npts, QObject *parent) :
  QcepDataGroup(name, parent)
{
  foreach (QString col, cols) {
    append(QcepDataColumnPtr(
             new QcepDataColumn(col, npts, parent)));
  }
}

