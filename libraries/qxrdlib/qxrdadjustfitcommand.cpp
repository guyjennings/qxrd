#include "qxrdadjustfitcommand.h"

QxrdAdjustFitCommand::QxrdAdjustFitCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdAdjustFitCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Fit to nearby peak when adding powder points?", NULL, NULL);
}
