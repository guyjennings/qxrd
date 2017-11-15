#ifndef QXRDFITCIRCLECOMMAND_H
#define QXRDFITCIRCLECOMMAND_H

#include "qxrdplotcommand.h"

class QxrdFitCircleCommand : public QxrdPlotCommand
{
public:
  QxrdFitCircleCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDFITCIRCLECOMMAND_H
