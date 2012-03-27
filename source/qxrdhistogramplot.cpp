#include "qxrdhistogramplot.h"

QxrdHistogramPlot::QxrdHistogramPlot(QWidget *parent) :
    QxrdPlot(parent)
{
}

void QxrdHistogramPlot::init(QxrdHistogramPlotSettingsWPtr settings)
{
  QxrdPlot::init(settings);

  insertLegend(m_Legend, QwtPlot::BottomLegend);
}
