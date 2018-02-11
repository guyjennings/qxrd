#ifndef QXRDADJUSTENERGYCOMMAND_H
#define QXRDADJUSTENERGYCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdAdjustEnergyCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdAdjustEnergyCommand(QxrdPlotWidget            *plot,
                          QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADJUSTENERGYCOMMAND_H
