#include "qxrdplot.h"

QxrdPlot::QxrdPlot(QWidget *parent)
  : QwtPlot(parent)
{
}

void QxrdPlot::autoScalePlot()
{
  setAxisAutoScale(QwtPlot::yLeft);
  setAxisAutoScale(QwtPlot::xBottom);

  replot();
}

