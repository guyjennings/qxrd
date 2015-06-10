#include "qcepdatacolumn.h"

QcepDataColumn::QcepDataColumn(QcepSettingsSaverWPtr saver, QString name, int npts, QcepDataObjectWPtr parent) :
  QcepDataObject(saver, name, parent),
  m_NPoints(npts)
{
  set_Type("Data Column");

  resize(m_NPoints);

  set_Description(tr("%1 rows").arg(m_NPoints));
}

