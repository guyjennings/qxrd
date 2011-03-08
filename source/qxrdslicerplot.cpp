#include "qxrdslicerplot.h"

QxrdSlicerPlot::QxrdSlicerPlot(QWidget *parent) :
    QxrdPlot(parent)
{
  insertLegend(m_Legend, QwtPlot::BottomLegend);

  autoScale();
}
