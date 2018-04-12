#include "qcepcontextseparatorcommand.h"
#include <QAction>

QcepContextSeparatorCommand::QcepContextSeparatorCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("separatorCommand", "", plot, set)
{

}

QAction* QcepContextSeparatorCommand::contextMenuAction(const QPoint & /*pos*/)
{
  QAction *res = newContextAction("", m_PlotWidget, NULL);

  res -> setSeparator(true);

  return res;
}
