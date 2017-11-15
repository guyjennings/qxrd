#ifndef QXRDZOOMINCOMMAND_H
#define QXRDZOOMINCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdZoomInCommand : public QxrdPlotButtonCommand
{
public:
  QxrdZoomInCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDZOOMINCOMMAND_H
