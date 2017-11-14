#ifndef QXRDMASKCIRCLESCOMMAND_H
#define QXRDMASKCIRCLESCOMMAND_H

#include "qxrdplotcommand.h"

class QxrdMaskCirclesCommand : public QxrdPlotCommand
{
public:
  QxrdMaskCirclesCommand(QString name);

  QToolButton *toolButton();
};

#endif // QXRDMASKCIRCLESCOMMAND_H
