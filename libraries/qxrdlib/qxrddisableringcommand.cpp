#include "qxrddisableringcommand.h"
#include "qxrdimageplot.h"

QxrdDisableRingCommand::QxrdDisableRingCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdDisableRingCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Disable Ring %1", NULL, NULL);
}
