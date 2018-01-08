#ifndef QXRDCOLORMAP_H
#define QXRDCOLORMAP_H

#include "qxrdlib_global.h"
#include <qwt_color_map.h>
#include "qxrdimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdColorMap : public QwtLinearColorMap
{
public:
  QxrdColorMap(QxrdImagePlotWidgetSettingsWPtr set);

  QxrdImagePlotWidgetSettingsWPtr settings();

private:
  QxrdImagePlotWidgetSettingsWPtr m_Settings;
};

#endif // QXRDCOLORMAP_H
