/******************************************************************
*
*  $Id: qxrdmaskrasterdata.h,v 1.3 2009/08/04 16:45:20 jennings Exp $
*
*******************************************************************/

#ifndef QXRDMASKRASTERDATA_H
#define QXRDMASKRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>

class QxrdMaskData;

class QxrdMaskRasterData : public QwtRasterData
{
public:
  QxrdMaskRasterData(QxrdMaskData *mask, int interp);
  QxrdMaskRasterData();

  double value(double x, double y) const;
  QwtDoubleInterval range() const;

  QxrdMaskRasterData *copy() const;

  /*  private: */
  /*   QxrdMaskRasterData(const QxrdMaskRasterData&); */

private:
  QxrdMaskData  *m_Mask;
  int            m_Interpolate;
  HEADER_IDENT("$Id: qxrdmaskrasterdata.h,v 1.3 2009/08/04 16:45:20 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdmaskrasterdata.h,v $
*  Revision 1.3  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

