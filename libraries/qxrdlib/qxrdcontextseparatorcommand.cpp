#include "qxrdcontextseparatorcommand.h"
#include <QAction>

QxrdContextSeparatorCommand::QxrdContextSeparatorCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdContextSeparatorCommand::contextMenuAction(const QPoint & /*pos*/)
{
  QAction *res = newContextAction("", m_PlotWidget, NULL);

  res -> setSeparator(true);

  return res;
}
