#ifndef QXRDSETCENTERCOMMAND_H
#define QXRDSETCENTERCOMMAND_H

#include "qxrdplotcommand.h"

class QxrdSetCenterCommand : public QxrdPlotCommand
{
public:
  QxrdSetCenterCommand(QString name);

  QToolButton *toolButton();
};

#endif // QXRDSETCENTERCOMMAND_H
