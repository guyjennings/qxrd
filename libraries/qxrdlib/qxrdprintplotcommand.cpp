#include "qxrdprintplotcommand.h"
#include "qxrdimageplot.h"

QxrdPrintPlotCommand::QxrdPrintPlotCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdPrintPlotCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Print Graph...",
                          m_PlotWidget, &QxrdPlotWidget::printGraph);
}
