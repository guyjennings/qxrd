#ifndef QXRDSLICECOMMAND_H
#define QXRDSLICECOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QxrdSliceCommand : public QxrdPlotButtonCommand
{
public:
  QxrdSliceCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDSLICECOMMAND_H
