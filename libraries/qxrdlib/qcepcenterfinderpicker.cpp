#include "qcepcenterfinderpicker.h"
#include "qcepimageplot.h"
#include "qceprasterdata.h"
#include "qwt_picker_machine.h"

QcepCenterFinderPicker::QcepCenterFinderPicker(QcepImagePlot *plot)
  : QcepImagePlotMeasurer(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragPointMachine());
  setRubberBand(QwtPicker::CrossRubberBand);
}
