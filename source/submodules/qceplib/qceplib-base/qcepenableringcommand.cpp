#include "qcepenableringcommand.h"

QcepEnableRingCommand::QcepEnableRingCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("enableRingCommand", "Enable Ring", plot, set)
{

}

QAction* QcepEnableRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Enable Ring %1", NULL, NULL);
}
