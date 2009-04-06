#include "qxrdmaskcolormap.h"

QxrdMaskColorMap::QxrdMaskColorMap(const QColor &deselcol, const QColor &selcol)
  : QwtColorMap(QwtColorMap::RGB),
    m_DeselectedColor(deselcol.rgba()),
    m_SelectedColor(selcol.rgba())
{
}

QxrdMaskColorMap::QxrdMaskColorMap()
  : QwtColorMap(QwtColorMap::RGB),
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

QRgb QxrdMaskColorMap::rgb(const QwtDoubleInterval &inter, double value) const
{
//   if (value > 0.5) {
//     return m_SelectedColor;
//   } else {
//     return m_DeselectedColor;
//   }

  int v = (1-value)*255.0;
  if (v<0) v=0;
  if (v>255) v=255;

  return qRgba(255,0,0, v);
}

unsigned char QxrdMaskColorMap::colorIndex(const QwtDoubleInterval &inter, double value) const
{
  return 0;
}
