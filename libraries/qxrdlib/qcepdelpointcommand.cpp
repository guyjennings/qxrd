#include "qcepdelpointcommand.h"

QcepDelPointCommand::QcepDelPointCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("deletePointCommand", "Delete point", plot, set)
{

}

QAction* QcepDelPointCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return newContextAction("Delete point at (%1,%2)", NULL, NULL);
}
