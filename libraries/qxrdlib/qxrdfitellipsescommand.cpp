#include "qxrdfitellipsescommand.h"

QxrdFitEllipsesCommand::QxrdFitEllipsesCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{

}

QAction* QxrdFitEllipsesCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Fit Ellipses to all powder rings", NULL, NULL);
}
