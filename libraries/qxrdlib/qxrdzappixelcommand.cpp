#include "qxrdzappixelcommand.h"

QxrdZapPixelCommand::QxrdZapPixelCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("zapPixelCommand", "Zap Pixel", plot, set)
{

}

QAction* QxrdZapPixelCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Zap (replace with avg of neighboring values) pixel [%1,%2]",
                          NULL, NULL);
}
