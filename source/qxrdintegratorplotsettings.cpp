#include "qxrdintegratorplotsettings.h"

QxrdIntegratorPlotSettings::QxrdIntegratorPlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdIntegratorPlotSettingsPtr QxrdIntegratorPlotSettings::newIntegratorPlotSettings()
{
  QxrdIntegratorPlotSettingsPtr set(new QxrdIntegratorPlotSettings("integratorPlot"));

  return set;
}
