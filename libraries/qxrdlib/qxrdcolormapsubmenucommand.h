#ifndef QXRDCOLORMAPSUBMENUCOMMAND_H
#define QXRDCOLORMAPSUBMENUCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdColorMapSubmenuCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdColorMapSubmenuCommand(QString                    name,
                             QxrdPlotWidget            *plot,
                             QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDCOLORMAPSUBMENUCOMMAND_H
