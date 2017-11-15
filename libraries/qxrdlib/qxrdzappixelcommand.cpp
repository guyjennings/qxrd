#include "qxrdzappixelcommand.h"

QxrdZapPixelCommand::QxrdZapPixelCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdZapPixelCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Zap (replace with avg of neighboring values) pixel [%1,%2]",
                          NULL, NULL);
}
