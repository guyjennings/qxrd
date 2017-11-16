#include "qxrdcontextseparatorcommand.h"

QxrdContextSeparatorCommand::QxrdContextSeparatorCommand(QString name, QxrdPlotWidget *plot)
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
