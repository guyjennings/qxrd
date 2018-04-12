#ifndef QXRDADJUSTENERGYCOMMAND_H
#define QXRDADJUSTENERGYCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdAdjustEnergyCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdAdjustEnergyCommand(QcepPlotWidget            *plot,
                          QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADJUSTENERGYCOMMAND_H
