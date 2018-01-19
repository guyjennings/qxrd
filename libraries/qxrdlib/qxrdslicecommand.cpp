#include "qxrdslicecommand.h"
#include <QToolButton>

QxrdSliceCommand::QxrdSliceCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/slice.png", "Slice", true)
{
}
