#ifndef QXRDTRACERINGCOMMAND_H
#define QXRDTRACERINGCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdTraceRingCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdTraceRingCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDTRACERINGCOMMAND_H
