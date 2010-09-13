/******************************************************************
*
*  $Id: qxrdmaskcolormap.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#include "qxrdmaskcolormap.h"

QxrdMaskColorMap::QxrdMaskColorMap(const QColor &deselcol, const QColor &selcol)
  : QwtLinearColorMap(QwtColorMap::RGB),
    m_DeselectedColor(deselcol.rgba()),
    m_SelectedColor(selcol.rgba()),
    SOURCE_IDENT("$Id: qxrdmaskcolormap.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $")
{
}

QxrdMaskColorMap::QxrdMaskColorMap()
  : QwtLinearColorMap(QwtColorMap::RGB),
    m_DeselectedColor(qRgba(255,0,0,255)),
    m_SelectedColor(qRgba(255,0,0,0))
{
}

QxrdMaskColorMap* QxrdMaskColorMap::copy() const
{
  QxrdMaskColorMap *map = new QxrdMaskColorMap();

  *map = *this;

  return map;
}

QRgb QxrdMaskColorMap::rgb(const QwtDoubleInterval &/*inter*/, double value) const
{
//   if (value > 0.5) {
//     return m_SelectedColor;
//   } else {
//     return m_DeselectedColor;
//   }

  int v = (int)((1.0-value)*255.0);
  if (v<0) v=0;
  if (v>255) v=255;

  return qRgba(255,0,0, v);
}

unsigned char QxrdMaskColorMap::colorIndex(const QwtDoubleInterval &/*inter*/, double /*value*/) const
{
  return 0;
}

/******************************************************************
*
*  $Log: qxrdmaskcolormap.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.5  2009/07/25 15:16:57  jennings
*  Made QxrdMaskColorMap descend from QwtLinearColorMap
*
*  Revision 1.4  2009/06/28 16:33:20  jennings
*  Eliminated compiler warnings
*
*  Revision 1.3  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

