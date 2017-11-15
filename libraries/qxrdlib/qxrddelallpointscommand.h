#ifndef QXRDDELALLPOINTSCOMMAND_H
#define QXRDDELALLPOINTSCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdDelAllPointsCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdDelAllPointsCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDELALLPOINTSCOMMAND_H
