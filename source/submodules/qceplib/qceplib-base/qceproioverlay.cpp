#include "qceproioverlay.h"

QcepROIOverlay::QcepROIOverlay(QString                    name,
                               QcepPlotWidget            *plot,
                               QcepPlotWidgetSettingsWPtr set,
                               QcepROIModelWPtr roic) :
  QcepPlotOverlay(name, plot, set),
  m_ROIModel(roic),
  m_ROICurves()
{
}
