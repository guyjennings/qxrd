/******************************************************************
*
*  $Id: qxrdrasterdata.h,v 1.10 2009/08/25 18:43:03 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRASTERDATA_H
#define QXRDRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"

class QxrdRasterData : public QwtRasterData
{
//  Q_OBJECT;

public:
  QxrdRasterData(QxrdDoubleImageData *img, int interp, QxrdMaskData *msk=NULL);
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
  QxrdDoubleImageData   *m_Data;
  QxrdMaskData          *m_Mask;
  int                    m_NRows;
  int                    m_NCols;
  QwtDoubleInterval      m_Range;
  int                    m_Interpolate;
  HEADER_IDENT("$Id: qxrdrasterdata.h,v 1.10 2009/08/25 18:43:03 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdrasterdata.h,v $
*  Revision 1.10  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.9  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.8  2009/07/25 17:03:40  jennings
*  More improvements to image plotting code
*
*  Revision 1.7  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

