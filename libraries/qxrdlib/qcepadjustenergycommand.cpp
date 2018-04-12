#include "qcepadjustenergycommand.h"
#include "qcepimageplot.h"

QcepAdjustEnergyCommand::QcepAdjustEnergyCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("adjustEnergyCommand", "Adjust Energy to match Calibrant Ring", plot, set)
{

}

QAction* QcepAdjustEnergyCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Adjust Energy to match Calibrant Ring %1", m_PlotWidget, NULL);
}
