#include "qcepplotoverlay.h"

QcepPlotOverlay::QcepPlotOverlay(
    QString                    name,
    QcepPlotWidget            *plot,
    QcepPlotWidgetSettingsWPtr set) :
  QcepObject(name),
  m_PlotWidget(plot),
  m_Settings(set)
{
}

void QcepPlotOverlay::setPen(QPen pen)
{
}

QcepPlotWidgetSettingsWPtr QcepPlotOverlay::settings() const
{
  return m_Settings;
}
