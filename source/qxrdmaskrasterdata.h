/******************************************************************
*
*  $Id: qxrdmaskrasterdata.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDMASKRASTERDATA_H
#define QXRDMASKRASTERDATA_H

#include "qcepmacros.h"

#include <qwt_raster_data.h>
#include "qxrdmaskdata.h"

class QxrdMaskRasterData : public QwtRasterData
{
public:
  QxrdMaskRasterData(QxrdMaskDataPtr mask = QxrdMaskDataPtr(), int interp = 0);

//private:
//  QxrdMaskRasterData();
//  QxrdMaskRasterData(const QxrdMaskRasterData &cpy);

public:
  double value(double x, double y) const;
  QwtDoubleInterval range() const;

  QxrdMaskRasterData* copy() const;

  const QxrdMaskDataPtr mask() const { return m_Mask; }
  int interp() const                 { return m_Interpolate; }

  /*  private: */
  /*   QxrdMaskRasterData(const QxrdMaskRasterData&); */

private:
  QxrdMaskDataPtr  m_Mask;
  int              m_Interpolate;
  HEADER_IDENT("$Id: qxrdmaskrasterdata.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdmaskrasterdata.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.3.4.4  2010/06/14 20:57:21  jennings
*  *** empty log message ***
*
*  Revision 1.3.4.3  2010/05/06 18:51:03  jennings
*  Removed some raster data constructors - replaced with default arguments to main constructor
*
*  Revision 1.3.4.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.3.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.3  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

