#include "qxrddelallpointscommand.h"

QxrdDelAllPointsCommand::QxrdDelAllPointsCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdDelAllPointsCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete all Rings", NULL, NULL);
}
