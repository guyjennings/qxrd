#include "qxrddelallpointscommand.h"

QxrdDelAllPointsCommand::QxrdDelAllPointsCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("deleteAllCommand", "Delete all Rings", plot, set)
{

}

QAction* QxrdDelAllPointsCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete all Rings", NULL, NULL);
}
