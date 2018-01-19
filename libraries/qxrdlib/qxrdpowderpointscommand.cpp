#include "qxrdpowderpointscommand.h"
#include <QToolButton>

QxrdPowderPointsCommand::QxrdPowderPointsCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/powder_point.png", "Powder Points", true)
{  
}
