#include "qcepdatacolumn.h"

QcepDataColumn::QcepDataColumn(QcepSettingsSaverWPtr saver, QString name, int npts) :
  QcepDataObject(saver, name),
  m_NPoints(npts)
{
  set_Type("Data Column");

  resize(m_NPoints);

  set_Description(tr("%1 rows").arg(m_NPoints));
}

QcepDataColumnPtr QcepDataColumn::newDataColumn(QcepSettingsSaverWPtr saver, QString name, int npts)
{
  QcepDataColumnPtr res(new QcepDataColumn(saver, name, npts));

  return res;
}
