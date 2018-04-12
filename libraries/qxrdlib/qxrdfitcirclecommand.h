#ifndef QXRDFITCIRCLECOMMAND_H
#define QXRDFITCIRCLECOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitCircleCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdFitCircleCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITCIRCLECOMMAND_H
