#ifndef QXRDMISSINGRINGCOMMAND_H
#define QXRDMISSINGRINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdMissingRingCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdMissingRingCommand(QxrdPlotWidget            *plot,
                         QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDMISSINGRINGCOMMAND_H
