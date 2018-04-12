#include "qxrdcontextseparatorcommand.h"
#include <QAction>

QxrdContextSeparatorCommand::QxrdContextSeparatorCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("separatorCommand", "", plot, set)
{

}

QAction* QxrdContextSeparatorCommand::contextMenuAction(const QPoint & /*pos*/)
{
  QAction *res = newContextAction("", m_PlotWidget, NULL);

  res -> setSeparator(true);

  return res;
}
