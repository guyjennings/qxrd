#ifndef QCEPMASKCOLORMAP_H
#define QCEPMASKCOLORMAP_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepimageplotwidgetsettings-ptr.h"
#include <qwt_color_map.h>

class QCEP_EXPORT QcepMaskColorMap : public QwtLinearColorMap
{
public:
  QcepMaskColorMap(QcepImagePlotWidgetSettingsWPtr set,
                   const QColor &deselcol, const QColor &selcol);
//  QcepMaskColorMap();

  //  QcepMaskColorMap* copy() const;

  QRgb rgb(const QwtInterval &inter, double value) const;
  unsigned char colorIndex(const QwtInterval &inter, double value) const;

private:
  QcepImagePlotWidgetSettingsWPtr m_Settings;
  QRgb                            m_DeselectedColor;
  QRgb                            m_SelectedColor;
};

#endif // QCEPMASKCOLORMAP_H
