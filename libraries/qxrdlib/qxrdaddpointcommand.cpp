#include "qxrdaddpointcommand.h"

QxrdAddPointCommand::QxrdAddPointCommand(QcepPlotWidget* plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("addPointCommand", "Add Point", plot, set)
{

}

QAction* QxrdAddPointCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Add point at (%1,%2)", NULL, NULL);
}
