#include "qxrdadjustdistancecommand.h"

QxrdAdjustDistanceCommand::QxrdAdjustDistanceCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{
}

QAction* QxrdAdjustDistanceCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Adjust Detector Distance to match Calibrant Ring %1",
                          NULL, NULL);
}
