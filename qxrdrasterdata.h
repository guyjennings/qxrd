/******************************************************************
*
*  $Id: qxrdrasterdata.h,v 1.8 2009/07/25 17:03:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRASTERDATA_H
#define QXRDRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qxrdimagedata.h"

class QxrdRasterData : public QwtRasterData
{
 public:
  QxrdRasterData(QxrdImageData *img, int interp);
  QxrdRasterData();

  double value(double x, double y) const;
  QxrdRasterData* copy() const;
  QwtDoubleInterval range() const;
  void setDisplayedRange(double min, double max);

  double minValue();
  double maxValue();

  void setInterpolate(int interp);
  int interpolate();

  int width() const;
  int height() const;

 private:
  QxrdImageData     *m_Data;
//  int                m_Offset;
  int                m_NRows;
  int                m_NCols;
  QwtDoubleInterval  m_Range;
  int                m_Interpolate;
  HEADER_IDENT("$Id: qxrdrasterdata.h,v 1.8 2009/07/25 17:03:40 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdrasterdata.h,v $
*  Revision 1.8  2009/07/25 17:03:40  jennings
*  More improvements to image plotting code
*
*  Revision 1.7  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

