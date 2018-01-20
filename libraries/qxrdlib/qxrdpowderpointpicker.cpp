#include "qxrdpowderpointpicker.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qwt_picker_machine.h"

QxrdPowderPointPicker::QxrdPowderPointPicker(QxrdImagePlot *plot)
  : QxrdImagePlotMeasurer(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerClickPointMachine());
  setRubberBand(QwtPicker::CrossRubberBand);
}
