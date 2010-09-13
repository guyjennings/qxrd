/******************************************************************
*
*  $Id: qxrdrasterdata.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRASTERDATA_H
#define QXRDRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrddoubleimagedata.h"

class QxrdRasterData;

typedef QSharedPointer<QxrdRasterData> QxrdRasterDataPtr;

class QxrdRasterData : public QwtRasterData
{
//  Q_OBJECT;

public:
  QxrdRasterData(QxrdDoubleImageDataPtr img = QxrdDoubleImageDataPtr(),
                 int interp = 1,
                 QxrdMaskDataPtr msk = QxrdMaskDataPtr(),
                 QwtDoubleInterval range = QwtDoubleInterval(0,40000));

//private:
//  QxrdRasterData();
//  QxrdRasterData(const QxrdRasterData& cpy);

public:
  double value(double x, double y) const;
  QxrdRasterData* copy() const;
  QwtDoubleInterval range() const;
  void setDisplayedRange(double min, double max);

  double minValue();
  double maxValue();

  void setInterpolate(int interp);
  int interpolate();

  QwtDoubleInterval percentileRange(double lowpct, double highpct);

  int width() const;
  int height() const;

  const QxrdDoubleImageDataPtr data() const { return m_Data; }
  const QxrdMaskDataPtr mask() const { return m_Mask; }
  int interp() const          { return m_Interpolate; }

private:
  QxrdDoubleImageDataPtr m_Data;
  QxrdMaskDataPtr        m_Mask;
  int                    m_NRows;
  int                    m_NCols;
  QwtDoubleInterval      m_Range;
  int                    m_Interpolate;
  HEADER_IDENT("$Id: qxrdrasterdata.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};


#endif

/******************************************************************
*
*  $Log: qxrdrasterdata.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/09/08 19:40:57  jennings
*  Added tilt controls to center finder
*  Disabled fit refine command (for now)
*  Added percentile display mode
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.10.4.4  2010/06/17 16:02:36  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*  Revision 1.10.4.3  2010/05/09 13:23:05  jennings
*  Fixed image display bug caused by incomplete implementation of QxrdRasterData::copy
*
*  Revision 1.10.4.2  2010/05/06 18:51:03  jennings
*  Removed some raster data constructors - replaced with default arguments to main constructor
*
*  Revision 1.10.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
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

