#include "qxrdroioverlay.h"

QxrdROIOverlay::QxrdROIOverlay(QString                    name,
                               QcepPlotWidget            *plot,
                               QcepPlotWidgetSettingsWPtr set,
                               QxrdROIModelWPtr           roic) :
  QcepPlotOverlay(name, plot, set),
  m_ROIModel(roic),
  m_ROICurves()
{
}
