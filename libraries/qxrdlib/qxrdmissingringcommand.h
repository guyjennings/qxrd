#ifndef QXRDMISSINGRINGCOMMAND_H
#define QXRDMISSINGRINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdMissingRingCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdMissingRingCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDMISSINGRINGCOMMAND_H
