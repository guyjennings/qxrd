#include "qxrdprintplotcommand.h"
#include "qxrdimageplot.h"

QxrdPrintPlotCommand::QxrdPrintPlotCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("printPlotCommand", "Print Graph", plot, set)
{

}

QAction* QxrdPrintPlotCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Print Graph...",
                          m_PlotWidget, &QxrdPlotWidget::printGraph);
}
