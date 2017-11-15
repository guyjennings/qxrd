#ifndef QXRDMASKCIRCLESCOMMAND_H
#define QXRDMASKCIRCLESCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QxrdMaskCirclesCommand : public QxrdPlotButtonCommand
{
public:
  QxrdMaskCirclesCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDMASKCIRCLESCOMMAND_H
