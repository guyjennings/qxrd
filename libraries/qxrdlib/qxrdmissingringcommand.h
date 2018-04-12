#ifndef QXRDMISSINGRINGCOMMAND_H
#define QXRDMISSINGRINGCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdMissingRingCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdMissingRingCommand(QcepPlotWidget            *plot,
                         QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDMISSINGRINGCOMMAND_H
