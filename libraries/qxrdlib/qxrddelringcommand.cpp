#include "qxrddelringcommand.h"
#include "qxrdimageplot.h"

QxrdDelRingCommand::QxrdDelRingCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdDelRingCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Delete Ring %1", m_Plot, NULL);
}
