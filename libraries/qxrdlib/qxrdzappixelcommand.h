#ifndef QXRDZAPPIXELCOMMAND_H
#define QXRDZAPPIXELCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdZapPixelCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdZapPixelCommand(QxrdPlotWidget            *plot,
                      QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDZAPPIXELCOMMAND_H
