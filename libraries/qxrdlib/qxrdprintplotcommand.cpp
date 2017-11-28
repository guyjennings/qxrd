#include "qxrdprintplotcommand.h"
#include "qxrdimageplot.h"

QxrdPrintPlotCommand::QxrdPrintPlotCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdPrintPlotCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Print Graph...",
                          m_Plot, &QxrdImagePlot::printGraph);
}
