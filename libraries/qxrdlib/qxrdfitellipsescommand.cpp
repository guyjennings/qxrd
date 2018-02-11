#include "qxrdfitellipsescommand.h"

QxrdFitEllipsesCommand::QxrdFitEllipsesCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("fitEllipsesCommand", "Fit Ellipses to all powder rings", plot, set)
{

}

QAction* QxrdFitEllipsesCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Ellipses to all powder rings", NULL, NULL);
}
