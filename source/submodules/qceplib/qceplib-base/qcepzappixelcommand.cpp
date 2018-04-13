#include "qcepzappixelcommand.h"

QcepZapPixelCommand::QcepZapPixelCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("zapPixelCommand", "Zap Pixel", plot, set)
{

}

QAction* QcepZapPixelCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Zap (replace with avg of neighboring values) pixel [%1,%2]",
                          NULL, NULL);
}
