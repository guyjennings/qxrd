#include "qxrddelallpointscommand.h"

QxrdDelAllPointsCommand::QxrdDelAllPointsCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdDelAllPointsCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete all Rings", NULL, NULL);
}
