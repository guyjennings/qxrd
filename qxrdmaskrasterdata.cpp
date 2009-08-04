/******************************************************************
*
*  $Id: qxrdmaskrasterdata.cpp,v 1.5 2009/08/04 16:45:20 jennings Exp $
*
*******************************************************************/

#include "qxrdmaskrasterdata.h"

#include "qxrdrasterdata.h"

QxrdMaskRasterData::QxrdMaskRasterData(QxrdMaskData *mask, int interp)
  : QwtRasterData(QwtDoubleRect(0,0,mask->get_Width(),mask->get_Height())),
    m_Mask(mask),
    m_Interpolate(interp),
    SOURCE_IDENT("$Id: qxrdmaskrasterdata.cpp,v 1.5 2009/08/04 16:45:20 jennings Exp $")
{
}

QxrdMaskRasterData::QxrdMaskRasterData()
  : m_Mask(NULL),
    m_Interpolate(0)
{
}

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
      return m_Mask->maskValue(((int) round(x)), ((int) round(y)));
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

/******************************************************************
*
*  $Log: qxrdmaskrasterdata.cpp,v $
*  Revision 1.5  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.4  2009/06/28 16:34:27  jennings
*  Fixed problems with copyMask which could result in image and mask dimensions getting out of sync.
*
*  Revision 1.3  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/


