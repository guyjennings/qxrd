#ifndef QXRDMASKCIRCLESCOMMAND_H
#define QXRDMASKCIRCLESCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdMaskCirclesCommand : public QxrdPlotButtonCommand
{
public:
  QxrdMaskCirclesCommand(QString name, QxrdPlotWidget *plot);

  QToolButton *toolButton();
};

#endif // QXRDMASKCIRCLESCOMMAND_H
