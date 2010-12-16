#include "qxrdmaskrasterdata.h"

#include "qxrdrasterdata.h"

QxrdMaskRasterData::QxrdMaskRasterData(QxrdMaskDataPtr mask, int interp)
  : QwtRasterData(QwtDoubleRect(0,0,(mask?mask->get_Width():0),(mask?mask->get_Height():0))),
    m_Mask(mask),
    m_Interpolate(interp)
{
  QCEP_DEBUG(DEBUG_IMAGES,
             printf("QxrdMaskRasterData::QxrdMaskRasterData(%p,%d) [%p]\n",
                    mask.data(), interp, this);
  );
}

//QxrdMaskRasterData::QxrdMaskRasterData()
//  : QxrdMaskRasterData(NULL, 0)
//{
//}
//
//QxrdMaskRasterData::QxrdMaskRasterData(const QxrdMaskRasterData& cpy)
//  : QxrdMaskRasterData(cpy.mask(), cpy.interp())
//{
//}
//
double QxrdMaskRasterData::value(double x, double y) const
{
  if (m_Mask) {
    if (m_Interpolate) {
      int ix = (int) x, iy = (int) y;
      double dx = x-ix, dy = y-iy;

      double f00 = m_Mask->maskValue(ix,iy);
      double f10 = m_Mask->maskValue(ix+1,iy);
      double f01 = m_Mask->maskValue(ix,iy+1);
      double f11 = m_Mask->maskValue(ix+1,iy+1);
      
      double f0 = f00*(1-dx)+f10*dx;
      double f1 = f01*(1-dx)+f11*dx;
      
      double f = f0*(1-dy)+f1*dy;
      
      return f;
    } else {
      return m_Mask->maskValue(((int) qRound(x)), ((int) qRound(y)));
    }
  } else {
    return 1;
  }
}

short int *QxrdMaskRasterData::data() const
{
  if (m_Mask) {
    return m_Mask->data();
  } else {
    return NULL;
  }
}

QwtDoubleInterval QxrdMaskRasterData::range() const
{
  return QwtDoubleInterval(0.0, 3.0);
}

QxrdMaskRasterData* QxrdMaskRasterData::copy() const
{
  QCEP_DEBUG(DEBUG_IMAGES,
             printf("QxrdMaskRasterData::copy() [%p]\n", this);
  );

  QxrdMaskRasterData *n = new QxrdMaskRasterData(this->mask(), this->interp());

  return n;
}
