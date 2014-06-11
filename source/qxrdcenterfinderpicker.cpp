#include "qxrdcenterfinderpicker.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdimageplot.h"
#include "qxrdrasterdata.h"
#include "qwt_picker_machine.h"

QxrdCenterFinderPicker::QxrdCenterFinderPicker(QxrdImagePlot *plot)
  : QxrdImagePlotMeasurer(plot -> canvas(), plot),
    m_Plot(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragPointMachine());
  setRubberBand(QwtPicker::CrossRubberBand);
}
