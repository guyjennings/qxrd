#include "qxrdnormalizeringscommand.h"

QxrdNormalizeRingsCommand::QxrdNormalizeRingsCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("normalizeRingsCommand", "Normalize Powder Rings", plot, set)
{

}

QAction* QxrdNormalizeRingsCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Normalize Powder Rings", NULL, NULL);
}
