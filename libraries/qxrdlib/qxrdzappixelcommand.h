#ifndef QXRDZAPPIXELCOMMAND_H
#define QXRDZAPPIXELCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdZapPixelCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdZapPixelCommand(QcepPlotWidget            *plot,
                      QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDZAPPIXELCOMMAND_H
