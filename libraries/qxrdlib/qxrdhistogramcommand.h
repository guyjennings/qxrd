#ifndef QXRDHISTOGRAMCOMMAND_H
#define QXRDHISTOGRAMCOMMAND_H

#include "qxrdplotcommand.h"

class QxrdHistogramCommand : public QxrdPlotCommand
{
public:
  QxrdHistogramCommand(QString name);

  QToolButton *toolButton();
};

#endif // QXRDHISTOGRAMCOMMAND_H
