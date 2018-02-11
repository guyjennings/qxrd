#include "qxrdfitcirclecommand.h"

QxrdFitCircleCommand::QxrdFitCircleCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("fitCircleCommand", "Fit Circle from Points on Ring", plot, set)
{
}

QAction* QxrdFitCircleCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Circle from Points on Ring %1", NULL, NULL);
}
