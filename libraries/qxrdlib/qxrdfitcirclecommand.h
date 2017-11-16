#ifndef QXRDFITCIRCLECOMMAND_H
#define QXRDFITCIRCLECOMMAND_H

#include "qxrdplotcommand.h"

class QXRD_EXPORT QxrdFitCircleCommand : public QxrdPlotCommand
{
public:
  QxrdFitCircleCommand(QString name, QxrdPlotWidget *plot);

  QToolButton *toolButton();
};

#endif // QXRDFITCIRCLECOMMAND_H
