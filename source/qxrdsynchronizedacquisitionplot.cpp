#include "qxrdsynchronizedacquisitionplot.h"

QxrdSynchronizedAcquisitionPlot::QxrdSynchronizedAcquisitionPlot(QWidget *parent) :
    QxrdPlot(parent)
{
}

void QxrdSynchronizedAcquisitionPlot::init(QxrdSynchronizedAcquisitionPlotSettingsWPtr settings)
{
  QxrdPlot::init(settings);

  insertLegend(m_Legend, QwtPlot::BottomLegend);
}
