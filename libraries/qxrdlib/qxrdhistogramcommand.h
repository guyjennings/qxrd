#ifndef QXRDHISTOGRAMCOMMAND_H
#define QXRDHISTOGRAMCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdHistogramCommand : public QxrdPlotButtonCommand
{
public:
  QxrdHistogramCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDHISTOGRAMCOMMAND_H
