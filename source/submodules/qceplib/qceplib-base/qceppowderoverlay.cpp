#include "qceppowderoverlay.h"

QcepPowderOverlay::QcepPowderOverlay(QString                    name,
                                     QcepPlotWidget            *plot,
                                     QcepPlotWidgetSettingsWPtr set,
                                     QcepPowderRingsModelWPtr   rings) :
  QcepPlotOverlay(name, plot, set),
  m_PowderRings(rings),
  m_PowderCurves()
{

}
