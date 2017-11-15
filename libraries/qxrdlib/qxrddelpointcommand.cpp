#include "qxrddelpointcommand.h"

QxrdDelPointCommand::QxrdDelPointCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdDelPointCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Delete point at (%1,%2)", NULL, NULL);
}
