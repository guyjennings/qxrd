#include "qcepdelringcommand.h"
#include "qcepimageplot.h"

QcepDelRingCommand::QcepDelRingCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("deleteRingCommand", "Delete Ring", plot, set)
{

}

QAction* QcepDelRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete Ring %1", m_PlotWidget, NULL);
}
