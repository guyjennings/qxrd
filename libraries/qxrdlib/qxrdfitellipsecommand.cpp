#include "qxrdfitellipsecommand.h"

QxrdFitEllipseCommand::QxrdFitEllipseCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("fitEllipseCommand", "Fit Ellipse from Points on Ring", plot, set)
{

}

QAction* QxrdFitEllipseCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Ellipse from Points on Ring %1", NULL, NULL);
}
