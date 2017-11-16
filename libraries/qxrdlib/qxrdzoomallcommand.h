#ifndef QXRDZOOMALLCOMMAND_H
#define QXRDZOOMALLCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdZoomAllCommand : public QxrdPlotButtonCommand
{
public:
  QxrdZoomAllCommand(QString name, QxrdPlotWidget *plot);

  QToolButton *toolButton();
};

#endif // QXRDZOOMALLCOMMAND_H
