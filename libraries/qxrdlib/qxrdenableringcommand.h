#ifndef QXRDENABLERINGCOMMAND_H
#define QXRDENABLERINGCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdEnableRingCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdEnableRingCommand(QcepPlotWidget            *plot,
                        QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDENABLERINGCOMMAND_H
