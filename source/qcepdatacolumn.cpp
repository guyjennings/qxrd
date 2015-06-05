#include "qcepdatacolumn.h"

QcepDataColumn::QcepDataColumn(QcepSettingsSaverWPtr saver, QString name, int npts, QObject *parent) :
  QcepDataObject(saver, name, parent),
  m_NPoints(npts)
{
  set_Type("column");

  resize(m_NPoints);
}

