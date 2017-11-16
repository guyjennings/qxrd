#ifndef QXRDMEASURECOMMAND_H
#define QXRDMEASURECOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdMeasureCommand : public QxrdPlotButtonCommand
{
public:
  QxrdMeasureCommand(QString name, QxrdPlotWidget *plot);

  QToolButton *toolButton();
};

#endif // QXRDMEASURECOMMAND_H
