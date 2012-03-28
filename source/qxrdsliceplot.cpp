#include "qxrdsliceplot.h"

QxrdSlicePlot::QxrdSlicePlot(QWidget *parent) :
    QxrdPlot(parent)
{
}

void QxrdSlicePlot::init(QxrdPlotSettingsWPtr settings)
{
  QxrdPlot::init(settings);

  if (m_Legend) {
    insertLegend(m_Legend, QwtPlot::BottomLegend);
  }
}
