#include "qxrdfitpeakcommand.h"

QxrdFitPeakCommand::QxrdFitPeakCommand(QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand("fitPeakCommand", "Fit Diffracted Peak near", plot, set)
{

}

QAction* QxrdFitPeakCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Diffracted Peak near (%1,%2) [%3,%4]", NULL, NULL);
}
