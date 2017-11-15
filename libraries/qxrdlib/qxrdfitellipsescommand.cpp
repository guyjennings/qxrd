#include "qxrdfitellipsescommand.h"

QxrdFitEllipsesCommand::QxrdFitEllipsesCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdFitEllipsesCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Fit Ellipses to all powder rings", NULL, NULL);
}
