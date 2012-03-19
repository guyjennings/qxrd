#include "qxrdsliceplot.h"

QxrdSlicePlot::QxrdSlicePlot(QWidget *parent) :
    QxrdPlot(parent)
{
  insertLegend(m_Legend, QwtPlot::BottomLegend);
}
