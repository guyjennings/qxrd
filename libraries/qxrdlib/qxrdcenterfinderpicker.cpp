#include "qxrdcenterfinderpicker.h"
#include "qcepimageplot.h"
#include "qceprasterdata.h"
#include "qwt_picker_machine.h"

QxrdCenterFinderPicker::QxrdCenterFinderPicker(QcepImagePlot *plot)
  : QxrdImagePlotMeasurer(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragPointMachine());
  setRubberBand(QwtPicker::CrossRubberBand);
}
