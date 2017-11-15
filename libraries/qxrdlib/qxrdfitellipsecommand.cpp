#include "qxrdfitellipsecommand.h"

QxrdFitEllipseCommand::QxrdFitEllipseCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdFitEllipseCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Fit Ellipse from Points on Ring %1", NULL, NULL);
}
