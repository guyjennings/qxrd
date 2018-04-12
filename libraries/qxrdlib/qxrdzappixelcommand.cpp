#include "qxrdzappixelcommand.h"

QxrdZapPixelCommand::QxrdZapPixelCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("zapPixelCommand", "Zap Pixel", plot, set)
{

}

QAction* QxrdZapPixelCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Zap (replace with avg of neighboring values) pixel [%1,%2]",
                          NULL, NULL);
}
