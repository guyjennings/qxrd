#ifndef QCEPCOLORMAP_H
#define QCEPCOLORMAP_H

#include "qceplib_global.h"
#include <qwt_color_map.h>
#include "qcepimageplotwidgetsettings-ptr.h"

class QCEP_EXPORT QcepColorMap : public QwtLinearColorMap
{
public:
  QcepColorMap(QcepImagePlotWidgetSettingsWPtr set);

  QcepImagePlotWidgetSettingsWPtr settings();

private:
  QcepImagePlotWidgetSettingsWPtr m_Settings;
};

#endif // QCEPCOLORMAP_H
