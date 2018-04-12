#include "qxrddisableringcommand.h"
#include "qcepimageplot.h"

QxrdDisableRingCommand::QxrdDisableRingCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("disableRingCommand", "Disable Ring", plot, set)
{

}

QAction* QxrdDisableRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Disable Ring %1", NULL, NULL);
}
