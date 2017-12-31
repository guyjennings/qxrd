#include "qxrdfitpeakcommand.h"

QxrdFitPeakCommand::QxrdFitPeakCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdFitPeakCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Diffracted Peak near (%1,%2) [%3,%4]", NULL, NULL);
}
