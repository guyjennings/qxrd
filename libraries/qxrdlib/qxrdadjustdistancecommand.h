#ifndef QXRDADJUSTDISTANCECOMMAND_H
#define QXRDADJUSTDISTANCECOMMAND_H

#include "qxrdplotcontextmenucommand.h"
#include "qxrdimageplot-ptr.h"

class QXRD_EXPORT QxrdAdjustDistanceCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdAdjustDistanceCommand(QString name, QxrdPlotWidget *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADJUSTDISTANCECOMMAND_H
