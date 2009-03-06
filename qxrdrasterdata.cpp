#include "qxrdrasterdata.h"

QxrdRasterData::QxrdRasterData(QVector<double> img, int offset, int nrows, int ncols)
  : m_Data(img),
    m_Offset(offset),
    m_NRows(nrows),
    m_NCols(ncols),
    m_Range(0,1000),
    m_Interpolate(0)
{
}

double QxrdRasterData::value(double x, double y) const
{
  return m_Data.at(m_Offset+((int) x) + ((int) y)*m_NCols);
}

QxrdRasterData* QxrdRasterData::copy() const
{
  return new QxrdRasterData(*this);
}

QwtDoubleInterval QxrdRasterData::range() const
{
  return m_Range;
}
