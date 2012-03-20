#include "qxrdpowderfitimageplot.h"
#include "qwt_plot_zoomer.h"

QxrdPowderFitImagePlot::QxrdPowderFitImagePlot(QWidget *parent) :
  QxrdImagePlot(parent)
{
}

void QxrdPowderFitImagePlot::init(QxrdImagePlotSettingsWPtr settings)
{
  QxrdImagePlot::init(settings);

  m_Zoomer->setEnabled(false);

  QxrdImagePlotSettingsPtr set(imagePlotSettings());

  if (set) {
    set->set_DisplayMinimumPctle(2.0);
    set->set_DisplayMaximumPctle(98.0);

    set->set_DisplayScalingMode(PercentileMode);
  }
}
