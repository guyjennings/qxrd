#include "qxrdcontextseparatorcommand.h"
#include <QAction>

QxrdContextSeparatorCommand::QxrdContextSeparatorCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("separatorCommand", "", plot, set)
{

}

QAction* QxrdContextSeparatorCommand::contextMenuAction(const QPoint & /*pos*/)
{
  QAction *res = newContextAction("", m_PlotWidget, NULL);

  res -> setSeparator(true);

  return res;
}
