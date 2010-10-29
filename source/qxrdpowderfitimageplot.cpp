#include "qxrdpowderfitimageplot.h"
#include "qwt_plot_zoomer.h"

QxrdPowderFitImagePlot::QxrdPowderFitImagePlot(QWidget *parent) :
    QxrdImagePlot(parent)
{
  m_Zoomer->setEnabled(false);

  set_DisplayMinimumPctle(2.0);
  set_DisplayMaximumPctle(98.0);

  set_DisplayScalingMode(PercentileMode);
}
