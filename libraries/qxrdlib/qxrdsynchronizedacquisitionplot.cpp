#include "qxrdsynchronizedacquisitionplot.h"
#include "qxrdsynchronizedacquisitionplotsettings.h"

QxrdSynchronizedAcquisitionPlot::QxrdSynchronizedAcquisitionPlot(QWidget *parent) :
    QcepPlot(parent)
{
}

void QxrdSynchronizedAcquisitionPlot::initialize(QxrdSynchronizedAcquisitionPlotSettingsWPtr settings)
{
  QcepPlot::initialize(settings);

  insertLegend(m_Legend, QwtPlot::BottomLegend);
}
