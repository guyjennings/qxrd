#include "qxrdsynchronizedacquisitionplotsettings.h"

QxrdSynchronizedAcquisitionPlotSettings::QxrdSynchronizedAcquisitionPlotSettings(QcepSettingsSaverWPtr saver, QObject *parent) :
  QcepPlotSettings("syncAcqPlot", saver, parent)
{
}
