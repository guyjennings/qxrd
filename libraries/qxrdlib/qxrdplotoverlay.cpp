#include "qxrdplotoverlay.h"

QxrdPlotOverlay::QxrdPlotOverlay(
    QString                    name,
    QxrdPlotWidget            *plot,
    QxrdPlotWidgetSettingsWPtr set) :
  QcepObject(name),
  m_PlotWidget(plot),
  m_Settings(set)
{
}

void QxrdPlotOverlay::setPen(QPen pen)
{
}

QxrdPlotWidgetSettingsWPtr QxrdPlotOverlay::settings() const
{
  return m_Settings;
}
