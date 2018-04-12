#include "qxrddelpointcommand.h"

QxrdDelPointCommand::QxrdDelPointCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("deletePointCommand", "Delete point", plot, set)
{

}

QAction* QxrdDelPointCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete point at (%1,%2)", NULL, NULL);
}
