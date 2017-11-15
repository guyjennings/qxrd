#ifndef QXRDZOOMOUTCOMMAND_H
#define QXRDZOOMOUTCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdZoomOutCommand : public QxrdPlotButtonCommand
{
public:
  QxrdZoomOutCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDZOOMOUTCOMMAND_H
