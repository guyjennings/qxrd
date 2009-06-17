#include "qxrdmaskrasterdata.h"

#include "qxrdrasterdata.h"

QxrdMaskRasterData::QxrdMaskRasterData(QxrdImageData *img, int interp)
  : m_Image(img),
    m_Interpolate(interp)
{
}

QxrdMaskRasterData::QxrdMaskRasterData()
  : m_Image(NULL),
    m_Interpolate(0)
{
}

double QxrdMaskRasterData::value(double x, double y) const
{
  if (m_Image) {
    if (m_Interpolate) {
      int ix = (int) x, iy = (int) y;
      double dx = x-ix, dy = y-iy;

      double f00 = m_Image->maskValue(ix,iy);
      double f10 = m_Image->maskValue(ix+1,iy);
      double f01 = m_Image->maskValue(ix,iy+1);
      double f11 = m_Image->maskValue(ix+1,iy+1);
      
      double f0 = f00*(1-dx)+f10*dx;
      double f1 = f01*(1-dx)+f11*dx;
      
      double f = f0*(1-dy)+f1*dy;
      
      return f;
    } else {
      return m_Image->maskValue(((int) round(x)), ((int) round(y)));
    }
  } else {
    return 1;
  }
}

QwtDoubleInterval QxrdMaskRasterData::range() const
{
  return QwtDoubleInterval(0.0, 1.0);
}

QxrdMaskRasterData* QxrdMaskRasterData::copy() const
{
  QxrdMaskRasterData *n = new QxrdMaskRasterData(*this);

  return n;
}

