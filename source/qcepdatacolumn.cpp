#include "qcepdatacolumn.h"

QcepDataColumn::QcepDataColumn(QcepSettingsSaverWPtr saver, QString name, int npts, QcepDataObjectWPtr parent) :
  QcepDataObject(saver, name, parent),
  m_NPoints(npts)
{
  set_Type("column");

  resize(m_NPoints);
}

