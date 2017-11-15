#ifndef QXRDMEASURECOMMAND_H
#define QXRDMEASURECOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QxrdMeasureCommand : public QxrdPlotButtonCommand
{
public:
  QxrdMeasureCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDMEASURECOMMAND_H
