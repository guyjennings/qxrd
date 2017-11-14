#ifndef QXRDZOOMINCOMMAND_H
#define QXRDZOOMINCOMMAND_H

#include "qxrdplotcommand.h"

class QxrdZoomInCommand : public QxrdPlotCommand
{
public:
  QxrdZoomInCommand(QString name);

  QToolButton *toolButton();
};

#endif // QXRDZOOMINCOMMAND_H
