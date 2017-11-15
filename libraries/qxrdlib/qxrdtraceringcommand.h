#ifndef QXRDTRACERINGCOMMAND_H
#define QXRDTRACERINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdTraceRingCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdTraceRingCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDTRACERINGCOMMAND_H
