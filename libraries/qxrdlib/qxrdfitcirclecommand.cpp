#include "qxrdfitcirclecommand.h"

QxrdFitCircleCommand::QxrdFitCircleCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{
}

QAction* QxrdFitCircleCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Fit Circle from Points on Ring %1", NULL, NULL);
}
