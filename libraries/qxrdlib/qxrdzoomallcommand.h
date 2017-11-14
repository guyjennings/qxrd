#ifndef QXRDZOOMALLCOMMAND_H
#define QXRDZOOMALLCOMMAND_H

#include "qxrdplotcommand.h"

class QxrdZoomAllCommand : public QxrdPlotCommand
{
public:
  QxrdZoomAllCommand(QString name);

  QToolButton *toolButton();
};

#endif // QXRDZOOMALLCOMMAND_H
