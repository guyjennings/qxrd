#include "qxrdpowderpointpicker.h"
#include "qcepimageplot.h"
#include "qceprasterdata.h"
#include "qwt_picker_machine.h"

QxrdPowderPointPicker::QxrdPowderPointPicker(QcepImagePlot *plot)
  : QxrdImagePlotMeasurer(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerClickPointMachine());
  setRubberBand(QwtPicker::CrossRubberBand);
}
