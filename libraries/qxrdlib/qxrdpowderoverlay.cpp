#include "qxrdpowderoverlay.h"

QxrdPowderOverlay::QxrdPowderOverlay(QString                    name,
                                     QxrdPlotWidget            *plot,
                                     QxrdPlotWidgetSettingsWPtr set,
                                     QxrdPowderRingsModelWPtr   rings) :
  QxrdPlotOverlay(name, plot, set),
  m_PowderRings(rings),
  m_PowderCurves()
{

}
