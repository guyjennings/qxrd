#include "qxrdhistogramplotsettings.h"

QxrdHistogramPlotSettings::QxrdHistogramPlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdHistogramPlotSettingsPtr QxrdHistogramPlotSettings::newHistogramPlotSettings()
{
  QxrdHistogramPlotSettingsPtr set(new QxrdHistogramPlotSettings("histogramPlot"));

  return set;
}
