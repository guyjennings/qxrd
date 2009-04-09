#include "qxrdrasterdata.h"

QxrdRasterData::QxrdRasterData(QxrdImageData *img, int interp)
  : QwtRasterData(QwtDoubleRect(0,0,img->width(),img->height())),
    m_Data(img),
    m_NRows((img ? img->width(): 0)),
    m_NCols((img ? img->height() : 0)),
    m_Range(40000,48000),
    m_Interpolate(interp)
{
}

QxrdRasterData::QxrdRasterData()
  : QwtRasterData(),
    m_Data(0),
    m_NRows(0),
    m_NCols(0),
    m_Range(0,1),
    m_Interpolate(1)
{
}

void QxrdRasterData::setInterpolate(int interp)
{
//  printf("%p->QxrdRasterData::setInterpolate(%d)\n", this, interp);

  m_Interpolate = interp;
}

int QxrdRasterData::interpolate()
{
  return m_Interpolate;
}

double QxrdRasterData::value(double x, double y) const
{
  if (m_Data) {
    if (x < 0 || x > m_NCols) return 0;
    if (y < 0 || y > m_NRows) return 0;

    if (m_Interpolate) {
      int ix = ((int) x), iy = ((int) y);
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
      return m_Data->value(((int) round(x)) , ((int) round(y)));
    }
  } else {
    return 0;
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
    bool *mask = m_Data->mask();
    bool first = 1;
    double min = 0;

    for (int i=1; i<npixels; i++) {
      bool msk = mask[i];
      if (msk) {
        double val = data[i];
        if (first) {
          min = val;
          first = 0;
        } else if (val<min) {
          min = val;
        }
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
    bool *mask = m_Data->mask();
    bool first = 1;
    double max = 0;

    for (int i=1; i<npixels; i++) {
      bool msk = mask[i];
      if (msk) {
        double val = data[i];
        if (first) {
          max = val;
          first = 0;
        } else if(val>max) {
          max = val;
        }
      }
    }

    return max;
  } else {
    return 0;
  }
}
