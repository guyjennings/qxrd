#include "qxrdcenterfinderpicker.h"
#include "qxrdimageplot.h"
#include "qceprasterdata.h"
#include "qwt_picker_machine.h"

QxrdCenterFinderPicker::QxrdCenterFinderPicker(QxrdImagePlot *plot)
  : QxrdImagePlotMeasurer(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragPointMachine());
  setRubberBand(QwtPicker::CrossRubberBand);
}
