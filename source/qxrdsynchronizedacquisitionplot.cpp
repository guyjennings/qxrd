#include "qxrdsynchronizedacquisitionplot.h"

QxrdSynchronizedAcquisitionPlot::QxrdSynchronizedAcquisitionPlot(QWidget *parent) :
    QcepPlot(parent)
{
}

void QxrdSynchronizedAcquisitionPlot::init(QxrdSynchronizedAcquisitionPlotSettingsWPtr settings)
{
  QcepPlot::init(settings);

  insertLegend(m_Legend, QwtPlot::BottomLegend);
}
