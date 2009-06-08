#include "qxrdplotslicer.h"

QxrdPlotSlicer::QxrdPlotSlicer(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot)
{
}
