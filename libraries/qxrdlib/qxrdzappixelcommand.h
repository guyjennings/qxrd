#ifndef QXRDZAPPIXELCOMMAND_H
#define QXRDZAPPIXELCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdZapPixelCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdZapPixelCommand(QString                    name,
                      QxrdPlotWidget            *plot,
                      QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDZAPPIXELCOMMAND_H
