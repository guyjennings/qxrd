#include "qxrdfitpeakcommand.h"

QxrdFitPeakCommand::QxrdFitPeakCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{

}

QAction* QxrdFitPeakCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Diffracted Peak near (%1,%2) [%3,%4]", NULL, NULL);
}
