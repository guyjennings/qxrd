#include "qxrdhistogramplot.h"
#include "qxrdhistogramplotsettings.h"

QxrdHistogramPlot::QxrdHistogramPlot(QWidget *parent) :
    QcepPlot(parent)
{
}

void QxrdHistogramPlot::initialize(QxrdHistogramPlotSettingsWPtr settings)
{
  QcepPlot::initialize(settings);

  insertLegend(m_Legend, QwtPlot::BottomLegend);
  setAxisTitle(QwtPlot::yLeft, "# Pixels");
  setAxisTitle(QwtPlot::xBottom, "Intensity (% of overflow value)");
}
