#include "qxrdadjustdistancecommand.h"

QxrdAdjustDistanceCommand::QxrdAdjustDistanceCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("adjustDistanceCommand", "Adjust Detector Distance to match Calibrant Ring", plot, set)
{
}

QAction* QxrdAdjustDistanceCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Adjust Detector Distance to match Calibrant Ring %1",
                          NULL, NULL);
}
