#include "qxrddelpointcommand.h"

QxrdDelPointCommand::QxrdDelPointCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdDelPointCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete point at (%1,%2)", NULL, NULL);
}
