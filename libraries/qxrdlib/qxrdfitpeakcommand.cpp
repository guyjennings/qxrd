#include "qxrdfitpeakcommand.h"

QxrdFitPeakCommand::QxrdFitPeakCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("fitPeakCommand", "Fit Diffracted Peak near", plot, set)
{

}

QAction* QxrdFitPeakCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Diffracted Peak near (%1,%2) [%3,%4]", NULL, NULL);
}
