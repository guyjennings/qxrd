#include "qxrdsynchronizedacquisitionplotsettings.h"

QxrdSynchronizedAcquisitionPlotSettings::QxrdSynchronizedAcquisitionPlotSettings(QString name) :
  QcepPlotSettings(name)
{
}

QxrdSynchronizedAcquisitionPlotSettings::QxrdSynchronizedAcquisitionPlotSettings() :
  QxrdSynchronizedAcquisitionPlotSettings("syncAcqPlot")
{
}
