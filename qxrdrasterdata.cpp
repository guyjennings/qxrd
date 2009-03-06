#include "qxrdrasterdata.h"

QxrdRasterData::QxrdRasterData(QVector<double> *img, int offset, int nrows, int ncols)
  : QwtRasterData(QwtDoubleRect(0,0,ncols,nrows)),
    m_Data(img),
    m_Offset(offset),
    m_NRows(nrows),
    m_NCols(ncols),
    m_Range(40000,48000),
    m_Interpolate(0)
{
}

QxrdRasterData::QxrdRasterData()
  : QwtRasterData(),
    m_Offset(0),
    m_NRows(0),
    m_NCols(0),
    m_Range(0,1),
    m_Interpolate(0)
{
}

double QxrdRasterData::value(double x, double y) const
{
  if (x < 0 || x > m_NCols) return 0;
  if (y < 0 || y > m_NRows) return 0;

  return m_Data->at(m_Offset+((int) x) + ((int) y)*m_NCols);
}

QxrdRasterData* QxrdRasterData::copy() const
{
  return new QxrdRasterData(*this);
}

QwtDoubleInterval QxrdRasterData::range() const
{
  return m_Range;
}

void QxrdRasterData::setDisplayedRange(double min, double max)
{
  m_Range = QwtDoubleInterval(min, max);
}

double QxrdRasterData::minValue()
{
  double *data = m_Data->data()+m_Offset;
  int npixels = m_NRows*m_NCols;
  double min=data[0];

  for (int i=1; i<npixels; i++) {
    if(data[i]<min) {
      min = data[i];
    }
  }

  return min;
}

double QxrdRasterData::maxValue()
{
  double *data = m_Data->data()+m_Offset;
  int npixels = m_NRows*m_NCols;
  double max=data[0];

  for (int i=1; i<npixels; i++) {
    if(data[i]>max) {
      max = data[i];
    }
  }

  return max;
}
