#include "qxrdadjustfitcommand.h"

QxrdAdjustFitCommand::QxrdAdjustFitCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("adjustFitCommand", "Fit to nearby peak when adding powder points", plot, set)
{

}

QAction* QxrdAdjustFitCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit to nearby peak when adding powder points?", NULL, NULL);
}
