#ifndef QXRDZOOMOUTCOMMAND_H
#define QXRDZOOMOUTCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdZoomOutCommand : public QxrdPlotButtonCommand
{
public:
  QxrdZoomOutCommand(QString name, QxrdPlotWidget *plot);

  QToolButton *toolButton();
};

#endif // QXRDZOOMOUTCOMMAND_H
