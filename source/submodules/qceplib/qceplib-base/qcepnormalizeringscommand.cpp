#include "qcepnormalizeringscommand.h"

QcepNormalizeRingsCommand::QcepNormalizeRingsCommand(QString name, QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("normalizeRingsCommand", "Normalize Powder Rings", plot, set)
{

}

QAction* QcepNormalizeRingsCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Normalize Powder Rings", NULL, NULL);
}
