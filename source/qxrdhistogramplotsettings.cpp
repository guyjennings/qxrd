#include "qxrdhistogramplotsettings.h"

QxrdHistogramPlotSettings::QxrdHistogramPlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdHistogramPlotSettings::QxrdHistogramPlotSettings() :
  QxrdHistogramPlotSettings("histogramPlot")
{
}
