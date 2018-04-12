#include "qceptraceringcommand.h"

QcepTraceRingCommand::QcepTraceRingCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("traceRingCommand", "Trace Diffracted Ring", plot, set)
{

}

QAction* QcepTraceRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Trace Diffracted Ring starting at (%1,%2) [%3,%4]",
                          NULL, NULL);
}
