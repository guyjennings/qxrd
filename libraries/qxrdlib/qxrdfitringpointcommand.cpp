#include "qxrdfitringpointcommand.h"
#include "qxrdimageplot.h"

QxrdFitRingPointCommand::QxrdFitRingPointCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdFitRingPointCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Point on Diffracted Ring near (%1,%2) [%3,%4]",
                          m_PlotWidget, NULL);
}
