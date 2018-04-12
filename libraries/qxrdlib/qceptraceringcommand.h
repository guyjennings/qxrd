#ifndef QCEPTRACERINGCOMMAND_H
#define QCEPTRACERINGCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepTraceRingCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepTraceRingCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPTRACERINGCOMMAND_H
