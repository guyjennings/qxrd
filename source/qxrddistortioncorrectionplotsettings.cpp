#include "qxrddistortioncorrectionplotsettings.h"

QxrdDistortionCorrectionPlotSettings::QxrdDistortionCorrectionPlotSettings(QString name) :
  QxrdImagePlotSettings(name)
{
}

QxrdDistortionCorrectionPlotSettings::QxrdDistortionCorrectionPlotSettings() :
  QxrdDistortionCorrectionPlotSettings("distortionCorrectionPlot")
{
}
