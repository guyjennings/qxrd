#include "qxrdzappixelcommand.h"

QxrdZapPixelCommand::QxrdZapPixelCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdZapPixelCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Zap (replace with avg of neighboring values) pixel [%1,%2]",
                          NULL, NULL);
}
