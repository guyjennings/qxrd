#ifndef QCEPAUTOSCALECOMMAND_H
#define QCEPAUTOSCALECOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QcepAutoScaleCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepAutoScaleCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPAUTOSCALECOMMAND_H
