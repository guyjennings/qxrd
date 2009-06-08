#include "qxrdcenterfinderpicker.h"
#include "qxrdcenterfinderdialog.h"

QxrdCenterFinderPicker::QxrdCenterFinderPicker(
    QwtPlotCanvas *canvas, QxrdImagePlot *plot, QxrdCenterFinderDialog *dlog)
  : QwtPlotPicker(canvas),
    m_Plot(plot),
    m_Dialog(dlog)
{
  setSelectionFlags(QwtPicker::PointSelection);
  setRubberBand(QwtPicker::CrossRubberBand);
}
