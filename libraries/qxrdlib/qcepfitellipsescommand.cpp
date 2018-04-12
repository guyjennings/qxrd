#include "qcepfitellipsescommand.h"

QcepFitEllipsesCommand::QcepFitEllipsesCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("fitEllipsesCommand", "Fit Ellipses to all powder rings", plot, set)
{

}

QAction* QcepFitEllipsesCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Ellipses to all powder rings", NULL, NULL);
}
