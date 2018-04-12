#include "qxrdenableringcommand.h"

QxrdEnableRingCommand::QxrdEnableRingCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("enableRingCommand", "Enable Ring", plot, set)
{

}

QAction* QxrdEnableRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Enable Ring %1", NULL, NULL);
}
