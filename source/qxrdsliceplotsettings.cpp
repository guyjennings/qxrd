#include "qxrdsliceplotsettings.h"

QxrdSlicePlotSettings::QxrdSlicePlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdSlicePlotSettingsPtr QxrdSlicePlotSettings::newSlicePlotSettings()
{
  QxrdSlicePlotSettingsPtr set(new QxrdSlicePlotSettings("slicePlot"));

  return set;
}
