#include "qxrdnormalizeringscommand.h"

QxrdNormalizeRingsCommand::QxrdNormalizeRingsCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdNormalizeRingsCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Normalize Powder Rings", NULL, NULL);
}
