#include "qxrdsliceplot.h"

QxrdSlicePlot::QxrdSlicePlot(QWidget *parent) :
    QcepPlot(parent)
{
}

void QxrdSlicePlot::initialize(QcepPlotSettingsWPtr settings)
{
  QcepPlot::initialize(settings);

  if (m_Legend) {
    insertLegend(m_Legend, QwtPlot::BottomLegend);
  }
}
