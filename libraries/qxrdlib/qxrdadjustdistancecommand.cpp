#include "qxrdadjustdistancecommand.h"

QxrdAdjustDistanceCommand::QxrdAdjustDistanceCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{
}

QAction* QxrdAdjustDistanceCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Adjust Detector Distance to match Calibrant Ring %1",
                          NULL, NULL);
}
