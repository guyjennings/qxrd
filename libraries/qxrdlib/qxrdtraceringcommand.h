#ifndef QXRDTRACERINGCOMMAND_H
#define QXRDTRACERINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdTraceRingCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdTraceRingCommand(QString                    name,
                       QxrdPlotWidget            *plot,
                       QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDTRACERINGCOMMAND_H
