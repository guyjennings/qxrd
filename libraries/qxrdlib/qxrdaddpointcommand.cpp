#include "qxrdaddpointcommand.h"

QxrdAddPointCommand::QxrdAddPointCommand(QString name, QxrdPlotWidget* plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdAddPointCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Add point at (%1,%2)", NULL, NULL);
}
