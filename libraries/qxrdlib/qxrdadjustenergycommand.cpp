#include "qxrdadjustenergycommand.h"
#include "qxrdimageplot.h"

QxrdAdjustEnergyCommand::QxrdAdjustEnergyCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("adjustEnergyCommand", "Adjust Energy to match Calibrant Ring", plot, set)
{

}

QAction* QxrdAdjustEnergyCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Adjust Energy to match Calibrant Ring %1", m_PlotWidget, NULL);
}
