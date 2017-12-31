#include "qxrddisableringcommand.h"
#include "qxrdimageplot.h"

QxrdDisableRingCommand::QxrdDisableRingCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdDisableRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Disable Ring %1", NULL, NULL);
}
