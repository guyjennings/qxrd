#include "qxrdcontextseparatorcommand.h"

QxrdContextSeparatorCommand::QxrdContextSeparatorCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdContextSeparatorCommand::contextMenuAction(const QPoint &pos)
{
  return NULL;
}

bool QxrdContextSeparatorCommand::contextMenuSeparator()
{
  return true;
}
