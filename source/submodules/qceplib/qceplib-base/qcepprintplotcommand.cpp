#include "qcepprintplotcommand.h"
#include "qcepimageplot.h"

QcepPrintPlotCommand::QcepPrintPlotCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("printPlotCommand", "Print Graph", plot, set)
{

}

QAction* QcepPrintPlotCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Print Graph...",
                          m_PlotWidget, &QcepPlotWidget::printGraph);
}
