#ifndef QXRDZOOMALLCOMMAND_H
#define QXRDZOOMALLCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QxrdZoomAllCommand : public QxrdPlotButtonCommand
{
public:
  QxrdZoomAllCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDZOOMALLCOMMAND_H
