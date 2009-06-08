#include "qxrdplotmeasurer.h"

QxrdPlotMeasurer::QxrdPlotMeasurer(QwtPlotCanvas *canvas, QxrdImagePlot *plot)
  : QwtPlotPicker(canvas),
    m_Plot(plot)
{
}
