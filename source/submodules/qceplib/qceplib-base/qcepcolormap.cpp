#include "qcepcolormap.h"

QcepColorMap::QcepColorMap(QcepImagePlotWidgetSettingsWPtr set)
  : QwtLinearColorMap(),
    m_Settings(set)
{
}

QcepImagePlotWidgetSettingsWPtr QcepColorMap::settings()
{
  return m_Settings;
}
