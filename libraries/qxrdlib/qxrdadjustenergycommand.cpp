#include "qxrdadjustenergycommand.h"
#include "qxrdimageplot.h"

QxrdAdjustEnergyCommand::QxrdAdjustEnergyCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdAdjustEnergyCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Adjust Energy to match Calibrant Ring %1", m_PlotWidget, NULL);
}
