#include "qcephistogramselector.h"
#include "qwt_picker_machine.h"

QcepHistogramSelector::QcepHistogramSelector(QcepPlot *plot) :
    QcepPlotMeasurer(plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragRectMachine());
  setRubberBand(QwtPicker::RectRubberBand);
}
