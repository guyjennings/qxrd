#ifndef QXRDPOWDERPOINTSCOMMAND_H
#define QXRDPOWDERPOINTSCOMMAND_H

#include "qxrdplotcommand.h"

class QxrdPowderPointsCommand : public QxrdPlotCommand
{
public:
  QxrdPowderPointsCommand(QString name);

  QToolButton *toolButton();
};

#endif // QXRDPOWDERPOINTSCOMMAND_H
