#include "qxrdnormalizeringscommand.h"

QxrdNormalizeRingsCommand::QxrdNormalizeRingsCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdNormalizeRingsCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Normalize Powder Rings", NULL, NULL);
}
