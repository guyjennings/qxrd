#include "qcepdisableringcommand.h"
#include "qcepimageplot.h"

QcepDisableRingCommand::QcepDisableRingCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("disableRingCommand", "Disable Ring", plot, set)
{

}

QAction* QcepDisableRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Disable Ring %1", NULL, NULL);
}
