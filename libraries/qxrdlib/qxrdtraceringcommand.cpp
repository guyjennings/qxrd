#include "qxrdtraceringcommand.h"

QxrdTraceRingCommand::QxrdTraceRingCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdTraceRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Trace Diffracted Ring starting at (%1,%2) [%3,%4]",
                          NULL, NULL);
}
