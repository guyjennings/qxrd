#include "qxrdfitellipsescommand.h"

QxrdFitEllipsesCommand::QxrdFitEllipsesCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdFitEllipsesCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Fit Ellipses to all powder rings", NULL, NULL);
}
