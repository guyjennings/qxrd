#include "qcepmissingringcommand.h"

QcepMissingRingCommand::QcepMissingRingCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("missingRingCommand", "Missing Diffracted Ring", plot, set)
{

}

QAction* QcepMissingRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Missing Diffracted Ring near (%1,%2)",
                          NULL, NULL);
}
