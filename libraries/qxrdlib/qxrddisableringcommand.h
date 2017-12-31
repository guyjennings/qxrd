#ifndef QXRDDISABLERINGCOMMAND_H
#define QXRDDISABLERINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdDisableRingCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdDisableRingCommand(QString                    name,
                         QxrdPlotWidget            *plot,
                         QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDISABLERINGCOMMAND_H
