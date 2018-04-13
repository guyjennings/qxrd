#include "qcepautoscalecommand.h"
#include "qcepplotwidget.h"
#include <QObject>

QcepAutoScaleCommand::QcepAutoScaleCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("autoScaleCommand", "Autoscale", plot, set)
{

}

QAction* QcepAutoScaleCommand::contextMenuAction(const QPoint & /*pos*/)
{
  //TODO: fix crash...

  return newContextAction("Autoscale", m_PlotWidget, &QcepPlotWidget::zoomAll);
}
