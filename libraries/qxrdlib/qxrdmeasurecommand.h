#ifndef QXRDMEASURECOMMAND_H
#define QXRDMEASURECOMMAND_H

#include "qxrdplotcommand.h"

class QxrdMeasureCommand : public QxrdPlotCommand
{
public:
  QxrdMeasureCommand(QString name);

  QToolButton *toolButton();
};

#endif // QXRDMEASURECOMMAND_H
