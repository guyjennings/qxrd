#ifndef QXRDZOOMINCOMMAND_H
#define QXRDZOOMINCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QxrdZoomInCommand : public QxrdPlotButtonCommand
{
public:
  QxrdZoomInCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDZOOMINCOMMAND_H
