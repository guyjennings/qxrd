#include "qxrdcenterfinderpicker.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"

QxrdCenterFinderPicker::QxrdCenterFinderPicker(QxrdImagePlotPtr plot)
  : QxrdImagePlotMeasurer(plot -> canvas(), plot),
    m_Plot(plot),
    SOURCE_IDENT("$Id: qxrdcenterfinderpicker.cpp,v 1.3 2010/09/17 16:21:51 jennings Exp $")
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::PointSelection);
  setRubberBand(QwtPicker::CrossRubberBand);
}
