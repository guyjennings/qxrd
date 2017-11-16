#include "qxrdadjustenergycommand.h"
#include "qxrdimageplot.h"

QxrdAdjustEnergyCommand::QxrdAdjustEnergyCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdAdjustEnergyCommand::contextMenuAction()
{
  return newContextAction("Adjust Energy to match Calibrant Ring %1", m_Plot, NULL);
}
