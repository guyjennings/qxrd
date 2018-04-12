#ifndef QXRDDISABLERINGCOMMAND_H
#define QXRDDISABLERINGCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdDisableRingCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdDisableRingCommand(QcepPlotWidget            *plot,
                         QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDISABLERINGCOMMAND_H
