#include "qxrdmissingringcommand.h"

QxrdMissingRingCommand::QxrdMissingRingCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdMissingRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Missing Diffracted Ring near (%1,%2)",
                          NULL, NULL);
}