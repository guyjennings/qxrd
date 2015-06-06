#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"

QcepDataColumnScan::QcepDataColumnScan(QcepSettingsSaverWPtr sav, QString name, QStringList cols, int npts, QcepDataObjectWPtr parent) :
  QcepDataGroup(sav, name, parent)
{
  set_Type("columnscan");

  foreach (QString col, cols) {
    append(QcepDataColumnPtr(
             new QcepDataColumn(saver(), col, npts, parent)));
  }
}

