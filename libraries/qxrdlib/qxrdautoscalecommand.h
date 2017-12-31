#ifndef QXRDAUTOSCALECOMMAND_H
#define QXRDAUTOSCALECOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdAutoScaleCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdAutoScaleCommand(QString                    name,
                       QxrdPlotWidget            *plot,
                       QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDAUTOSCALECOMMAND_H
