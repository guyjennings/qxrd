#include "qxrdnormalizeringscommand.h"

QxrdNormalizeRingsCommand::QxrdNormalizeRingsCommand(QString name, QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("normalizeRingsCommand", "Normalize Powder Rings", plot, set)
{

}

QAction* QxrdNormalizeRingsCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Normalize Powder Rings", NULL, NULL);
}
