#include "qxrdintegratorplotsettings.h"

QxrdIntegratorPlotSettings::QxrdIntegratorPlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdIntegratorPlotSettings::QxrdIntegratorPlotSettings() :
  QxrdIntegratorPlotSettings("integratorPlot")
{
}
