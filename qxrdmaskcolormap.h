#ifndef QXRDMASKCOLORMAP_H
#define QXRDMASKCOLORMAP_H

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
};

#endif
