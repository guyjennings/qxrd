#include "qxrdautoscalecommand.h"
#include "qxrdplotwidget.h"
#include <QObject>

QxrdAutoScaleCommand::QxrdAutoScaleCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("autoScaleCommand", "Autoscale", plot, set)
{

}

QAction* QxrdAutoScaleCommand::contextMenuAction(const QPoint & /*pos*/)
{
  //TODO: fix crash...

  return newContextAction("Autoscale", m_PlotWidget, &QxrdPlotWidget::zoomAll);
}
