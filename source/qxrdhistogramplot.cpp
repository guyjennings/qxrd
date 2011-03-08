#include "qxrdhistogramplot.h"

QxrdHistogramPlot::QxrdHistogramPlot(QWidget *parent) :
    QxrdPlot(parent)
{
  insertLegend(m_Legend, QwtPlot::BottomLegend);

  autoScale();
}
