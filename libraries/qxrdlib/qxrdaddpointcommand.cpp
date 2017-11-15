#include "qxrdaddpointcommand.h"

QxrdAddPointCommand::QxrdAddPointCommand(QString name, QxrdImagePlot* plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdAddPointCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Add point at (%1,%2)", NULL, NULL);
}
