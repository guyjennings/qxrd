#include "qxrdcenterfinderplotsettings.h"

QxrdCenterFinderPlotSettings::QxrdCenterFinderPlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdCenterFinderPlotSettingsPtr QxrdCenterFinderPlotSettings::newCenterFinderPlotSettings()
{
  QxrdCenterFinderPlotSettingsPtr set(new QxrdCenterFinderPlotSettings("centeringPlot"));

  return set;
}
