#ifndef QXRDFITCIRCLECOMMAND_H
#define QXRDFITCIRCLECOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitCircleCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdFitCircleCommand(QString name, QxrdPlotWidget *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITCIRCLECOMMAND_H
