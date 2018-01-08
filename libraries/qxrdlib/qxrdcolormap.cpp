#include "qxrdcolormap.h"

QxrdColorMap::QxrdColorMap(QxrdImagePlotWidgetSettingsWPtr set)
  : QwtLinearColorMap(),
    m_Settings(set)
{
}

QxrdImagePlotWidgetSettingsWPtr QxrdColorMap::settings()
{
  return m_Settings;
}
