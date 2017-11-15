#ifndef QXRDAUTOSCALECOMMAND_H
#define QXRDAUTOSCALECOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdAutoScaleCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdAutoScaleCommand(QString name, QxrdImagePlot* plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDAUTOSCALECOMMAND_H
