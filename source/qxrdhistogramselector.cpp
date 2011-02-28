#include "qxrdhistogramselector.h"

QxrdHistogramSelector::QxrdHistogramSelector(QwtPlotCanvas *canvas, QxrdPlot *plot) :
    QxrdPlotMeasurer(canvas, plot)
{
  setTrackerMode(QwtPicker::AlwaysOn);
  setSelectionFlags(QwtPicker::RectSelection);
  setRubberBand(QwtPicker::RectRubberBand);
}
