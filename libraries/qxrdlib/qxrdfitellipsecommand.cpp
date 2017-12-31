#include "qxrdfitellipsecommand.h"

QxrdFitEllipseCommand::QxrdFitEllipseCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdFitEllipseCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Ellipse from Points on Ring %1", NULL, NULL);
}
