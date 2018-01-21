#include "qxrdplotoverlay.h"

QxrdPlotOverlay::QxrdPlotOverlay(
    QString                    name,
    QxrdPlotWidgetWPtr         plot,
    QxrdPlotWidgetSettingsWPtr set) :
  QcepObject(name),
  m_PlotWidget(plot),
  m_Settings(set)
{
}

void QxrdPlotOverlay::setPen(const QPen &pen)
{
}
