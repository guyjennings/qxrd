#include "qxrdfitellipsescommand.h"

QxrdFitEllipsesCommand::QxrdFitEllipsesCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("fitEllipsesCommand", "Fit Ellipses to all powder rings", plot, set)
{

}

QAction* QxrdFitEllipsesCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Ellipses to all powder rings", NULL, NULL);
}
