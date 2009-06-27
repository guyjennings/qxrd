/******************************************************************
*
*  $Id: qxrdmaskcolormap.h,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDMASKCOLORMAP_H
#define QXRDMASKCOLORMAP_H

#include "qcepmacros.h"

#include <qwt_color_map.h>

class QxrdMaskColorMap : public QwtColorMap
{
 public:
  QxrdMaskColorMap(const QColor &deselcol, const QColor &selcol);
  QxrdMaskColorMap();

  QxrdMaskColorMap *copy() const;

  QRgb rgb(const QwtDoubleInterval &inter, double value) const;
  unsigned char colorIndex(const QwtDoubleInterval &inter, double value) const;

 private:
  QRgb  m_DeselectedColor;
  QRgb  m_SelectedColor;
  HEADER_IDENT("$Id: qxrdmaskcolormap.h,v 1.2 2009/06/27 22:50:32 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdmaskcolormap.h,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

