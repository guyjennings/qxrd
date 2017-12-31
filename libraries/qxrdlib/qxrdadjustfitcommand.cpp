#include "qxrdadjustfitcommand.h"

QxrdAdjustFitCommand::QxrdAdjustFitCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdAdjustFitCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit to nearby peak when adding powder points?", NULL, NULL);
}
