#include "qxrdenableringcommand.h"

QxrdEnableRingCommand::QxrdEnableRingCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("enableRingCommand", "Enable Ring", plot, set)
{

}

QAction* QxrdEnableRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Enable Ring %1", NULL, NULL);
}
