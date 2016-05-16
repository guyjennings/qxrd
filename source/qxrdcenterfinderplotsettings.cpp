#include "qxrdcenterfinderplotsettings.h"

QxrdCenterFinderPlotSettings::QxrdCenterFinderPlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdCenterFinderPlotSettings::QxrdCenterFinderPlotSettings() :
  QxrdCenterFinderPlotSettings("centeringPlot")
{
}
