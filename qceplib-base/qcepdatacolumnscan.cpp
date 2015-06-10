#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"

QcepDataColumnScan::QcepDataColumnScan(QcepSettingsSaverWPtr sav, QString name, QStringList cols, int npts, QcepDataObjectWPtr parent) :
  QcepDataGroup(sav, name, parent),
  m_NumPoints(sav, this, "numPoints", 0, "Number of points in scan")
{
  set_Type("Data Column Scan");

  foreach (QString col, cols) {
    append(QcepDataColumnPtr(
             new QcepDataColumn(saver(), col, npts, parent)));
  }

  set_NumPoints(npts);

  set_Description(tr("%1 rows x %2 cols").arg(npts).arg(cols.count()));
}
