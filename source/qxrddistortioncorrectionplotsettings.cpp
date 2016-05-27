#include "qxrddistortioncorrectionplotsettings.h"

QxrdDistortionCorrectionPlotSettings::QxrdDistortionCorrectionPlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdDistortionCorrectionPlotSettingsPtr QxrdDistortionCorrectionPlotSettings::newDistortionCorrectionPlotSettings()
{
  QxrdDistortionCorrectionPlotSettingsPtr set(new QxrdDistortionCorrectionPlotSettings("distortionCorrectionPlot"));

  return set;
}
