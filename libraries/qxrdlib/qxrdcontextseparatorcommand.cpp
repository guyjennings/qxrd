#include "qxrdcontextseparatorcommand.h"
#include <QAction>

QxrdContextSeparatorCommand::QxrdContextSeparatorCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdContextSeparatorCommand::contextMenuAction(const QPoint &pos)
{
  QAction *res = newContextAction("", m_Plot, NULL);

  res -> setSeparator(true);

  return res;
}
