/******************************************************************
*
*  $Id: qxrdmaskcolormap.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDMASKCOLORMAP_H
#define QXRDMASKCOLORMAP_H

#include "qcepmacros.h"

#include <qwt_color_map.h>

class QxrdMaskColorMap : public QwtLinearColorMap
{
 public:
  QxrdMaskColorMap(const QColor &deselcol, const QColor &selcol);
  QxrdMaskColorMap();

  QxrdMaskColorMap* copy() const;

  QRgb rgb(const QwtDoubleInterval &inter, double value) const;
  unsigned char colorIndex(const QwtDoubleInterval &inter, double value) const;

 private:
  QRgb  m_DeselectedColor;
  QRgb  m_SelectedColor;
  HEADER_IDENT("$Id: qxrdmaskcolormap.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdmaskcolormap.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:40  jennings
*  Moving files into source subdirectory
*
*  Revision 1.3.4.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.3.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.3  2009/07/25 15:16:57  jennings
*  Made QxrdMaskColorMap descend from QwtLinearColorMap
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

