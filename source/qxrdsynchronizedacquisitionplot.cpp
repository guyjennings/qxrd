#include "qxrdsynchronizedacquisitionplot.h"

QxrdSynchronizedAcquisitionPlot::QxrdSynchronizedAcquisitionPlot(QWidget *parent) :
    QxrdPlot(parent)
{
  insertLegend(m_Legend, QwtPlot::BottomLegend);
}
