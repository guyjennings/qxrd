#include "qxrdenableringcommand.h"

QxrdEnableRingCommand::QxrdEnableRingCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdEnableRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Enable Ring %1", NULL, NULL);
}
