/******************************************************************
*
*  $Id: qxrdmaskrasterdata.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#include "qxrdmaskrasterdata.h"

#include "qxrdrasterdata.h"

QxrdMaskRasterData::QxrdMaskRasterData(QxrdMaskDataPtr mask, int interp)
  : QwtRasterData(QwtDoubleRect(0,0,(mask?mask->get_Width():0),(mask?mask->get_Height():0))),
    m_Mask(mask),
    m_Interpolate(interp),
    SOURCE_IDENT("$Id: qxrdmaskrasterdata.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $")
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

QwtDoubleInterval QxrdMaskRasterData::range() const
{
  return QwtDoubleInterval(0.0, 1.0);
}

QxrdMaskRasterData* QxrdMaskRasterData::copy() const
{
  QCEP_DEBUG(DEBUG_IMAGES,
             printf("QxrdMaskRasterData::copy() [%p]\n", this);
  );

  QxrdMaskRasterData *n = new QxrdMaskRasterData(this->mask(), this->interp());

  return n;
}

/******************************************************************
*
*  $Log: qxrdmaskrasterdata.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:40  jennings
*  Moving files into source subdirectory
*
*  Revision 1.6.2.3  2010/05/17 15:59:53  jennings
*  Changed debugging output to use QCEP_DEBUG macro
*
*  Revision 1.6.2.2  2010/05/06 18:51:03  jennings
*  Removed some raster data constructors - replaced with default arguments to main constructor
*
*  Revision 1.6.2.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.6  2009/11/02 20:19:27  jennings
*  Changes to make it work with VC compiler
*
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


