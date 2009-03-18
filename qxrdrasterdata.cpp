#include "qxrdrasterdata.h"

QxrdRasterData::QxrdRasterData(QxrdImageData *img)
  : QwtRasterData(QwtDoubleRect(0,0,img->width(),img->height())),
    m_Data(img),
//    m_Offset(offset),
    m_NRows(img->width()),
    m_NCols(img->height()),
    m_Range(40000,48000),
    m_Interpolate(1)
{
}

QxrdRasterData::QxrdRasterData()
  : QwtRasterData(),
//    m_Offset(0),
    m_NRows(0),
    m_NCols(0),
    m_Range(0,1),
    m_Interpolate(1)
{
}

double QxrdRasterData::value(double x, double y) const
{
  if (x < 0 || x > m_NCols) return 0;
  if (y < 0 || y > m_NRows) return 0;

  if (m_Interpolate) {
     int ix = x, iy = y;
     double dx = x-ix, dy = y-iy;

     double f00 = m_Data->value((ix)   , (iy));
     double f10 = m_Data->value((ix+1) , (iy));
     double f01 = m_Data->value((ix)   , (iy+1));
     double f11 = m_Data->value((ix+1) , (iy+1));

     double f0 = f00*(1-dx)+f10*dx;
     double f1 = f01*(1-dx)+f11*dx;

     double f = f0*(1-dy)+f1*dy;

     return f;
   } else {
     return m_Data->value(((int) x) , ((int) y));
   }
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
  int npixels = m_NRows*m_NCols;

  if (m_Data) {
    double *data = m_Data->data();
    double min=data[0];

    for (int i=1; i<npixels; i++) {
      double val = data[i];
      if(val<min) {
        min = val;
      }
    }

    return min;
  } else {
    return 0;
  }
}

double QxrdRasterData::maxValue()
{
  int npixels = m_NRows*m_NCols;

  if (m_Data) {
    double *data = m_Data->data();
    double max=data[0];

    for (int i=1; i<npixels; i++) {
      double val = data[i];
      if(val>max) {
        max = val;
      }
    }

    return max;
  } else {
    return 0;
  }
}
