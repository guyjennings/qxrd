#include "qcepdelallpointscommand.h"

QcepDelAllPointsCommand::QcepDelAllPointsCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("deleteAllCommand", "Delete all Rings", plot, set)
{

}

QAction* QcepDelAllPointsCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete all Rings", NULL, NULL);
}
