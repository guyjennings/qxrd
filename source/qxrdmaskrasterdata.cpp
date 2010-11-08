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
    return m_Mask->defaultValue();
  }
}

short int *QxrdMaskRasterData::data() const
{
  return m_Mask->data();
}

void QxrdMaskRasterData::mask_combine(QxrdMaskRasterData *a)
{
  if (m_Mask && a) {
    int nrow = m_Mask->get_Height();
    int ncol = m_Mask->get_Width();
    long npix = nrow*ncol;

    short int *dst = m_Mask->data();
    short int *src = a->data();

    for (long i=0; i<npix; i++) {
      int sv = *src;
      int dv = *dst++;

      *src++ = i & 0x03;/*(sv & 0x0002) | (dv & 0x0001);*/
    }
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
