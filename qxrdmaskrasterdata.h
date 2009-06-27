/******************************************************************
*
*  $Id: qxrdmaskrasterdata.h,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDMASKRASTERDATA_H
#define QXRDMASKRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>

class QxrdImageData;

class QxrdMaskRasterData : public QwtRasterData
{
public:
  QxrdMaskRasterData(QxrdImageData *img, int interp);
  QxrdMaskRasterData();

  double value(double x, double y) const;
  QwtDoubleInterval range() const;

  QxrdMaskRasterData *copy() const;

  /*  private: */
  /*   QxrdMaskRasterData(const QxrdMaskRasterData&); */

private:
  QxrdImageData *m_Image;
  int            m_Interpolate;
  HEADER_IDENT("$Id: qxrdmaskrasterdata.h,v 1.2 2009/06/27 22:50:32 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdmaskrasterdata.h,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

