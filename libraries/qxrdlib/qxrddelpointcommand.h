#ifndef QXRDDELPOINTCOMMAND_H
#define QXRDDELPOINTCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdDelPointCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdDelPointCommand(QString name, QxrdImagePlot* plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDELPOINTCOMMAND_H
