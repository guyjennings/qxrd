#ifndef QXRDADJUSTENERGYCOMMAND_H
#define QXRDADJUSTENERGYCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdAdjustEnergyCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdAdjustEnergyCommand(QString name, QxrdPlotWidget *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADJUSTENERGYCOMMAND_H
