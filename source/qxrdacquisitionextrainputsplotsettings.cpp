#include "qxrdacquisitionextrainputsplotsettings.h"

QxrdAcquisitionExtraInputsPlotSettings::QxrdAcquisitionExtraInputsPlotSettings(QString name)
  : QcepPlotSettings(name)
{
}

QxrdAcquisitionExtraInputsPlotSettingsPtr QxrdAcquisitionExtraInputsPlotSettings::newAcquisitionExtraInputsPlotSettings()
{
  QxrdAcquisitionExtraInputsPlotSettingsPtr set(new QxrdAcquisitionExtraInputsPlotSettings("extraInputsPlot"));

  return set;
}

