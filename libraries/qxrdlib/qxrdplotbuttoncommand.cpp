#include "qxrdplotbuttoncommand.h"

QxrdPlotButtonCommand::QxrdPlotButtonCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotCommand(name, plot)
{

}

QAction* QxrdPlotButtonCommand::contextMenuAction(const QPoint &pos)
{
  return NULL;
}

bool QxrdPlotButtonCommand::contextMenuSeparator()
{
  return false;
}
