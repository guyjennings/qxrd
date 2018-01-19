#include "qxrdsetcentercommand.h"
#include <QToolButton>

QxrdSetCenterCommand::QxrdSetCenterCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/ringfinder.png", "Set Center", true)
{  
}
