#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"

QcepDataColumnScan::QcepDataColumnScan(QcepSettingsSaverWPtr sav, QString name) :
  QcepDataGroup(sav, name),
  m_NumPoints(sav, this, "numPoints", 0, "Number of points in scan")
{
  set_Type("Data Column Scan");
}

QcepDataColumnScanPtr QcepDataColumnScan::newDataColumnScan(QcepSettingsSaverWPtr sav, QString name, QStringList cols, int npts)
{
  QcepDataColumnScanPtr res(new QcepDataColumnScan(sav, name));

  foreach (QString col, cols) {
    res -> append(QcepDataColumn::newDataColumn(sav, col, npts));
  }

  res -> set_NumPoints(npts);
  res -> set_Description(tr("%1 rows x %2 cols").arg(npts).arg(cols.count()));

  return res;
}
