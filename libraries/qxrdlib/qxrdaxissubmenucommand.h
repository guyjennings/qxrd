#ifndef QXRDAXISSUBMENUCOMMAND_H
#define QXRDAXISSUBMENUCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdAxisSubmenuCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdAxisSubmenuCommand(QxrdPlotWidget            *plot,
                         QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDAXISSUBMENUCOMMAND_H
