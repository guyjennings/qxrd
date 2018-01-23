#include "qxrdroioverlay.h"

QxrdROIOverlay::QxrdROIOverlay(QString                    name,
                               QxrdPlotWidget            *plot,
                               QxrdPlotWidgetSettingsWPtr set,
                               QxrdROIModelWPtr           roic) :
  QxrdPlotOverlay(name, plot, set),
  m_ROIModel(roic),
  m_ROICurves()
{
}
