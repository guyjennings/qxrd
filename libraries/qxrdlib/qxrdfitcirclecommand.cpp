#include "qxrdfitcirclecommand.h"

QxrdFitCircleCommand::QxrdFitCircleCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{
}

QAction* QxrdFitCircleCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Circle from Points on Ring %1", NULL, NULL);
}
