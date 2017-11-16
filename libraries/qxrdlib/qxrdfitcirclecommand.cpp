#include "qxrdfitcirclecommand.h"

QxrdFitCircleCommand::QxrdFitCircleCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotCommand(name, plot)
{
}

QToolButton* QxrdFitCircleCommand::toolButton()
{
  return NULL;
}
