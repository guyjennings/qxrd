#include "qcepaddpointcommand.h"

QcepAddPointCommand::QcepAddPointCommand(QcepPlotWidget* plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("addPointCommand", "Add Point", plot, set)
{

}

QAction* QcepAddPointCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Add point at (%1,%2)", NULL, NULL);
}
