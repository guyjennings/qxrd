#include "qxrdroioverlay.h"

QxrdROIOverlay::QxrdROIOverlay(QString                         name,
                               QxrdPlotWidget                 *plot,
                               QxrdPlotWidgetSettingsWPtr      set,
                               QxrdROICoordinatesListModelWPtr roic) :
  QxrdPlotOverlay(name, plot, set),
  m_ROICoordsModel(roic),
  m_ROICurves()
{
}
