#ifndef QXRDMASKCOLORMAP_H
#define QXRDMASKCOLORMAP_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <qwt_color_map.h>

class QXRD_EXPORT QxrdMaskColorMap : public QwtLinearColorMap
{
 public:
  QxrdMaskColorMap(const QColor &deselcol, const QColor &selcol);
  QxrdMaskColorMap();

//  QxrdMaskColorMap* copy() const;

  QRgb rgb(const QwtInterval &inter, double value) const;
  unsigned char colorIndex(const QwtInterval &inter, double value) const;

 private:
  QRgb  m_DeselectedColor;
  QRgb  m_SelectedColor;
};

#endif