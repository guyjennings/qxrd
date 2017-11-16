#ifndef QXRDMISSINGRINGCOMMAND_H
#define QXRDMISSINGRINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdMissingRingCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdMissingRingCommand(QString name, QxrdPlotWidget *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDMISSINGRINGCOMMAND_H
