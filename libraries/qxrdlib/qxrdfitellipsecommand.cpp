#include "qxrdfitellipsecommand.h"

QxrdFitEllipseCommand::QxrdFitEllipseCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("fitEllipseCommand", "Fit Ellipse from Points on Ring", plot, set)
{

}

QAction* QxrdFitEllipseCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Ellipse from Points on Ring %1", NULL, NULL);
}
