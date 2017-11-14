#ifndef QXRDZOOMOUTCOMMAND_H
#define QXRDZOOMOUTCOMMAND_H

#include "qxrdplotcommand.h"

class QxrdZoomOutCommand : public QxrdPlotCommand
{
public:
  QxrdZoomOutCommand(QString name);

  QToolButton *toolButton();
};

#endif // QXRDZOOMOUTCOMMAND_H
