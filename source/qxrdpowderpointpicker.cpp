#include "qxrdpowderpointpicker.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"

QxrdPowderPointPicker::QxrdPowderPointPicker(QxrdImagePlot *plot)
  : QxrdImagePlotMeasurer(plot -> canvas(), plot),
    m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PointSelection);
  setRubberBand(QwtPicker::CrossRubberBand);
}
