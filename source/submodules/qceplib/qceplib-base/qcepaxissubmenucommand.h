#ifndef QCEPAXISSUBMENUCOMMAND_H
#define QCEPAXISSUBMENUCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepAxisSubmenuCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepAxisSubmenuCommand(QcepPlotWidget            *plot,
                         QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPAXISSUBMENUCOMMAND_H
