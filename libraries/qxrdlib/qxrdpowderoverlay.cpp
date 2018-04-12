#include "qxrdpowderoverlay.h"

QxrdPowderOverlay::QxrdPowderOverlay(QString                    name,
                                     QcepPlotWidget            *plot,
                                     QcepPlotWidgetSettingsWPtr set,
                                     QxrdPowderRingsModelWPtr   rings) :
  QcepPlotOverlay(name, plot, set),
  m_PowderRings(rings),
  m_PowderCurves()
{

}
