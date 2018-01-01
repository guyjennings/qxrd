#ifndef QXRDSCALINGSUBMENUCOMMAND_H
#define QXRDSCALINGSUBMENUCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdScalingSubmenuCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdScalingSubmenuCommand(QString                    name,
                            QxrdPlotWidget            *plot,
                            QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDSCALINGSUBMENUCOMMAND_H
