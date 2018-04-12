#include "qxrdtraceringcommand.h"

QxrdTraceRingCommand::QxrdTraceRingCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("traceRingCommand", "Trace Diffracted Ring", plot, set)
{

}

QAction* QxrdTraceRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Trace Diffracted Ring starting at (%1,%2) [%3,%4]",
                          NULL, NULL);
}
