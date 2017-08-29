#include "qxrdsliceplot.h"

QxrdSlicePlot::QxrdSlicePlot(QWidget *parent) :
    QcepPlot(parent)
{
}

void QxrdSlicePlot::init(QcepPlotSettingsWPtr settings)
{
  QcepPlot::init(settings);

  if (m_Legend) {
    insertLegend(m_Legend, QwtPlot::BottomLegend);
  }
}
