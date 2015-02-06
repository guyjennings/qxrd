#include "qxrdsynchronizedacquisitionplotsettings.h"

QxrdSynchronizedAcquisitionPlotSettings::QxrdSynchronizedAcquisitionPlotSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QxrdPlotSettings("syncAcqPlot", saver, parent)
{
}
