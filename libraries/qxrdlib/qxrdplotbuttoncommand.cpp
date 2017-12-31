#include "qxrdplotbuttoncommand.h"

QxrdPlotButtonCommand::QxrdPlotButtonCommand(QString                    name,
                                             QxrdPlotWidget            *plot,
                                             QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotCommand(name, plot, set)
{

}

QAction* QxrdPlotButtonCommand::contextMenuAction(const QPoint & /*pos*/)
{
  return NULL;
}
