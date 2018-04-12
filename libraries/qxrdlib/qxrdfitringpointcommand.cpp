#include "qxrdfitringpointcommand.h"
#include "qcepimageplot.h"

QxrdFitRingPointCommand::QxrdFitRingPointCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("fitRingPointCommand", "Fit Point on Diffracted Ring", plot, set)
{

}

QAction* QxrdFitRingPointCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Point on Diffracted Ring near (%1,%2) [%3,%4]",
                          m_PlotWidget, NULL);
}
