#include "qxrdcenterfinderpicker.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"

QxrdCenterFinderPicker::QxrdCenterFinderPicker(QxrdImagePlot *plot)
  : QxrdImagePlotMeasurer(plot -> canvas(), plot),
    m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PointSelection);
  setRubberBand(QwtPicker::CrossRubberBand);
}
