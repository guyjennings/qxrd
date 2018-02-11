#include "qxrddelringcommand.h"
#include "qxrdimageplot.h"

QxrdDelRingCommand::QxrdDelRingCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("deleteRingCommand", "Delete Ring", plot, set)
{

}

QAction* QxrdDelRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete Ring %1", m_PlotWidget, NULL);
}
