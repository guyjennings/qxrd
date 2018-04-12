#include "qcepfitcirclecommand.h"

QcepFitCircleCommand::QcepFitCircleCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("fitCircleCommand", "Fit Circle from Points on Ring", plot, set)
{
}

QAction* QcepFitCircleCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Circle from Points on Ring %1", NULL, NULL);
}
