#include "qxrdsynchronizedacquisitionplotsettings.h"

QxrdSynchronizedAcquisitionPlotSettings::QxrdSynchronizedAcquisitionPlotSettings(QcepSettingsSaverWPtr saver, QObject *parent) :
  QxrdPlotSettings("syncAcqPlot", saver, parent)
{
}
