#include "qxrdimageplotzoomer.h"
#include "qxrdimageplot.h"

QxrdImagePlotZoomer::QxrdImagePlotZoomer(QWidget *canvas, QxrdImagePlot *plot)
  : QxrdPlotZoomer(canvas, plot),
    m_ImagePlot(plot)
{
}
