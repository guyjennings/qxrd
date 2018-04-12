#include "qxrddelringcommand.h"
#include "qxrdimageplot.h"

QxrdDelRingCommand::QxrdDelRingCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("deleteRingCommand", "Delete Ring", plot, set)
{

}

QAction* QxrdDelRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete Ring %1", m_PlotWidget, NULL);
}
