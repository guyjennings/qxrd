#ifndef QXRDADJUSTDISTANCECOMMAND_H
#define QXRDADJUSTDISTANCECOMMAND_H

#include "qxrdplotcontextmenucommand.h"
#include "qxrdimageplot-ptr.h"

class QxrdAdjustDistanceCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdAdjustDistanceCommand(QString name, QxrdImagePlot* plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADJUSTDISTANCECOMMAND_H
