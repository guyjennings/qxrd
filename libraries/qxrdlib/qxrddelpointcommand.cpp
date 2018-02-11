#include "qxrddelpointcommand.h"

QxrdDelPointCommand::QxrdDelPointCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("deletePointCommand", "Delete point", plot, set)
{

}

QAction* QxrdDelPointCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete point at (%1,%2)", NULL, NULL);
}
