#ifndef QXRDENABLERINGCOMMAND_H
#define QXRDENABLERINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdEnableRingCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdEnableRingCommand(QString                    name,
                        QxrdPlotWidget            *plot,
                        QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDENABLERINGCOMMAND_H
