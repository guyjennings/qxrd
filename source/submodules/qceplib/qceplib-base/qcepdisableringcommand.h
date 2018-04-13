#ifndef QCEPDISABLERINGCOMMAND_H
#define QCEPDISABLERINGCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepDisableRingCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepDisableRingCommand(QcepPlotWidget            *plot,
                         QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPDISABLERINGCOMMAND_H
