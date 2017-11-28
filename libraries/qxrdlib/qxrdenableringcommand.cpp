#include "qxrdenableringcommand.h"

QxrdEnableRingCommand::QxrdEnableRingCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdEnableRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Enable Ring %1", NULL, NULL);
}
