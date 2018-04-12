#include "qxrdadjustenergycommand.h"
#include "qcepimageplot.h"

QxrdAdjustEnergyCommand::QxrdAdjustEnergyCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("adjustEnergyCommand", "Adjust Energy to match Calibrant Ring", plot, set)
{

}

QAction* QxrdAdjustEnergyCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Adjust Energy to match Calibrant Ring %1", m_PlotWidget, NULL);
}
