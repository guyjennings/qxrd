#ifndef QXRDADJUSTENERGYCOMMAND_H
#define QXRDADJUSTENERGYCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdAdjustEnergyCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdAdjustEnergyCommand(QString name, QxrdImagePlot* plot);

  QAction *contextMenuAction();
};

#endif // QXRDADJUSTENERGYCOMMAND_H
