#ifndef QXRDENABLERINGCOMMAND_H
#define QXRDENABLERINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdEnableRingCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdEnableRingCommand(QString name, QxrdPlotWidget *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDENABLERINGCOMMAND_H
