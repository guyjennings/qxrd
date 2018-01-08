#include "qxrdmaskcolormap.h"

QxrdMaskColorMap::QxrdMaskColorMap(
    QxrdImagePlotWidgetSettingsWPtr set,
    const QColor &deselcol, const QColor &selcol)
  : QwtLinearColorMap(QwtColorMap::RGB),
    m_Settings(set),
    m_DeselectedColor(deselcol.rgba()),
    m_SelectedColor(selcol.rgba())
{
}

//QxrdMaskColorMap::QxrdMaskColorMap()
//  : QwtLinearColorMap(QwtColorMap::RGB),
//  m_DeselectedColor(qRgba(255,0,0,255)),
//  m_SelectedColor(qRgba(255,0,0,0))
//{
//}

//QxrdMaskColorMap* QxrdMaskColorMap::copy() const
//{
//  QxrdMaskColorMap *map = new QxrdMaskColorMap();

//  *map = *this;

//  return map;
//}

QRgb QxrdMaskColorMap::rgb(const QwtInterval &/*inter*/, double value) const
{
  if (value > 0.5) {
    return m_SelectedColor;
  } else {
    return m_DeselectedColor;
  }

  //  int v = value;

  //  if (v & 1) {
  //    return qRgba(255,0,0,0);
  //  } else if (v & 2) {
  //    return qRgba(0,255,0,255);
  //  } else {
  //    return qRgba(255,0,0,255);
  //  }
  //  int v = (int)((1.0-value)*255.0);
  //  if (v<0) v=0;
  //  if (v>255) v=255;
  //
  //  return qRgba(255,0,0, v);
}

unsigned char QxrdMaskColorMap::colorIndex(const QwtInterval &/*inter*/, double /*value*/) const
{
  return 0;
}
