#include "qxrdsliceplotsettings.h"

QxrdSlicePlotSettings::QxrdSlicePlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdSlicePlotSettings::QxrdSlicePlotSettings()
  : QxrdSlicePlotSettings("slicePlot")
{
}
