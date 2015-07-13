#include "qxrdhistogramplot.h"

QxrdHistogramPlot::QxrdHistogramPlot(QWidget *parent) :
    QcepPlot(parent)
{
}

void QxrdHistogramPlot::init(QxrdHistogramPlotSettingsWPtr settings)
{
  QcepPlot::init(settings);

  insertLegend(m_Legend, QwtPlot::BottomLegend);
  setAxisTitle(QwtPlot::yLeft, "# Pixels");
  setAxisTitle(QwtPlot::xBottom, "Intensity (% of overflow value)");
}
