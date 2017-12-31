#include "qxrdmissingringcommand.h"

QxrdMissingRingCommand::QxrdMissingRingCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdMissingRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Missing Diffracted Ring near (%1,%2)",
                          NULL, NULL);
}
