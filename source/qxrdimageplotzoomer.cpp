#include "qxrdimageplotzoomer.h"
#include "qxrdimageplot.h"

QxrdImagePlotZoomer::QxrdImagePlotZoomer(QWidget *canvas, QxrdImagePlot *plot)
  : QcepPlotZoomer(canvas, plot),
    m_ImagePlot(plot)
{
}
