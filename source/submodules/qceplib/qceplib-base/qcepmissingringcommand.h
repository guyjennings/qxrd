#ifndef QCEPMISSINGRINGCOMMAND_H
#define QCEPMISSINGRINGCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepMissingRingCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepMissingRingCommand(QcepPlotWidget            *plot,
                         QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPMISSINGRINGCOMMAND_H
