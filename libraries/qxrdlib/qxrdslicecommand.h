#ifndef QXRDSLICECOMMAND_H
#define QXRDSLICECOMMAND_H

#include "qxrdplotcommand.h"

class QxrdSliceCommand : public QxrdPlotCommand
{
public:
  QxrdSliceCommand(QString name);

  QToolButton *toolButton();
};

#endif // QXRDSLICECOMMAND_H
