#include "qxrdautoscalecommand.h"
#include "qxrdimageplot.h"
#include <QObject>

QxrdAutoScaleCommand::QxrdAutoScaleCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdAutoScaleCommand::contextMenuAction(const QPoint &pos)
{
  return newContextAction("Autoscale", m_Plot, &QxrdImagePlot::autoScale);
}
