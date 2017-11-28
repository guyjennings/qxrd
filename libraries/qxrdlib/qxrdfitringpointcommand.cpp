#include "qxrdfitringpointcommand.h"
#include "qxrdimageplot.h"

QxrdFitRingPointCommand::QxrdFitRingPointCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdFitRingPointCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Point on Diffracted Ring near (%1,%2) [%3,%4]",
                          m_Plot, NULL);
}
