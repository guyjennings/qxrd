#include "qxrdaddpointcommand.h"

QxrdAddPointCommand::QxrdAddPointCommand(QxrdPlotWidget* plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("addPointCommand", "Add Point", plot, set)
{

}

QAction* QxrdAddPointCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Add point at (%1,%2)", NULL, NULL);
}
