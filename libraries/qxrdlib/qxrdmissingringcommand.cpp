#include "qxrdmissingringcommand.h"

QxrdMissingRingCommand::QxrdMissingRingCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("missingRingCommand", "Missing Diffracted Ring", plot, set)
{

}

QAction* QxrdMissingRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Missing Diffracted Ring near (%1,%2)",
                          NULL, NULL);
}
