#include "qxrdmeasurecommand.h"
#include <QToolButton>

QxrdMeasureCommand::QxrdMeasureCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/measure.png", "Measure", true)
{
}
