#include "qxrdhistogramselector.h"
#include "qwt_picker_machine.h"

QxrdHistogramSelector::QxrdHistogramSelector(QcepPlot *plot) :
    QcepPlotMeasurer(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragRectMachine());
  setRubberBand(QwtPicker::RectRubberBand);
}
