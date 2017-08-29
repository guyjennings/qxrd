#include "qxrdhistogramselector.h"
#include "qwt_picker_machine.h"

QxrdHistogramSelector::QxrdHistogramSelector(QWidget *canvas, QcepPlot *plot) :
    QcepPlotMeasurer(canvas, plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setStateMachine(new QwtPickerDragRectMachine());
  setRubberBand(QwtPicker::RectRubberBand);
}
