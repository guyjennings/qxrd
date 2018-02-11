#include "qxrdadjustfitcommand.h"

QxrdAdjustFitCommand::QxrdAdjustFitCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("adjustFitCommand", "Fit to nearby peak when adding powder points", plot, set)
{

}

QAction* QxrdAdjustFitCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit to nearby peak when adding powder points?", NULL, NULL);
}
