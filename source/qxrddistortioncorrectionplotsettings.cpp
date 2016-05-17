#include "qxrddistortioncorrectionplotsettings.h"

QxrdDistortionCorrectionPlotSettings::QxrdDistortionCorrectionPlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdDistortionCorrectionPlotSettings::QxrdDistortionCorrectionPlotSettings() :
  QxrdDistortionCorrectionPlotSettings("distortionCorrectionPlot")
{
}
