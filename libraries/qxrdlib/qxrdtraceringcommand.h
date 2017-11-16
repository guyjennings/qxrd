#ifndef QXRDTRACERINGCOMMAND_H
#define QXRDTRACERINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdTraceRingCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdTraceRingCommand(QString name, QxrdPlotWidget *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDTRACERINGCOMMAND_H
