#include "qxrddebug.h"
#include "qxrdmaskrasterdata.h"

#include "qxrdrasterdata.h"
#include "qxrdapplication.h"

QxrdMaskRasterData::QxrdMaskRasterData(QxrdMaskDataPtr mask, int interp)
  : QwtRasterData(/*QRectF(0,0,(mask?mask->get_Width():0),(mask?mask->get_Height():0))*/),
    m_Mask(mask),
    m_Interpolate(interp)
{
  if (g_Application && qcepDebug(DEBUG_IMAGES)) {
    g_Application->printMessage(QObject::tr("QxrdMaskRasterData::QxrdMaskRasterData(%1,%2) [%3]")
                                .HEXARG(mask.data()).arg(interp).HEXARG(this));
  }
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

QwtInterval QxrdMaskRasterData::range() const
{
  return QwtInterval(0.0, 3.0);
}

QxrdMaskRasterData* QxrdMaskRasterData::copy() const
{
  if (g_Application && qcepDebug(DEBUG_IMAGES)) {
    g_Application->printMessage(QObject::tr("QxrdMaskRasterData::copy() [%1]").HEXARG((void*) this));
  }

  QxrdMaskRasterData *n = new QxrdMaskRasterData(this->mask(), this->interp());

  return n;
}
