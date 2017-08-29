#include "qxrdsynchronizedacquisitionplotsettings.h"

QxrdSynchronizedAcquisitionPlotSettings::QxrdSynchronizedAcquisitionPlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdSynchronizedAcquisitionPlotSettingsPtr QxrdSynchronizedAcquisitionPlotSettings::newSynchronizedAcquisitionPlotSettings()
{
  QxrdSynchronizedAcquisitionPlotSettingsPtr set(new QxrdSynchronizedAcquisitionPlotSettings("syncAcqPlot"));

  return set;
}
