#include "qxrddelringcommand.h"
#include "qxrdimageplot.h"

QxrdDelRingCommand::QxrdDelRingCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdDelRingCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete Ring %1", m_PlotWidget, NULL);
}
