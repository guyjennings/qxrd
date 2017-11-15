#include "qxrdfitcirclecommand.h"

QxrdFitCircleCommand::QxrdFitCircleCommand(QString name, QxrdImagePlot *plot)
  : QxrdPlotCommand(name, plot)
{
}

QToolButton* QxrdFitCircleCommand::toolButton()
{
  return NULL;
}
