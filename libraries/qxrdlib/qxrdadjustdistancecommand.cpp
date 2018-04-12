#include "qxrdadjustdistancecommand.h"

QxrdAdjustDistanceCommand::QxrdAdjustDistanceCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("adjustDistanceCommand", "Adjust Detector Distance to match Calibrant Ring", plot, set)
{
}

QAction* QxrdAdjustDistanceCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Adjust Detector Distance to match Calibrant Ring %1",
                          NULL, NULL);
}
