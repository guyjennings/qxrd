#include "qceppowderpointpicker.h"
#include "qcepimageplot.h"
#include "qceprasterdata.h"
#include "qwt_picker_machine.h"

QcepPowderPointPicker::QcepPowderPointPicker(QcepImagePlot *plot)
  : QcepImagePlotMeasurer(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerClickPointMachine());
  setRubberBand(QwtPicker::CrossRubberBand);
}
