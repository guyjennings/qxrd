#ifndef QCEPADJUSTENERGYCOMMAND_H
#define QCEPADJUSTENERGYCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepAdjustEnergyCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepAdjustEnergyCommand(QcepPlotWidget            *plot,
                          QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPADJUSTENERGYCOMMAND_H
