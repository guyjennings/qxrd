#include "qxrddelallpointscommand.h"

QxrdDelAllPointsCommand::QxrdDelAllPointsCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("deleteAllCommand", "Delete all Rings", plot, set)
{

}

QAction* QxrdDelAllPointsCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete all Rings", NULL, NULL);
}
