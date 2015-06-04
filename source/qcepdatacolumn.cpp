#include "qcepdatacolumn.h"

QcepDataColumn::QcepDataColumn(QString name, int npts, QObject *parent) :
  QcepDataObject(name, parent),
  m_NPoints(npts)
{
  resize(m_NPoints);
}

