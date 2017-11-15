#ifndef QXRDENABLERINGCOMMAND_H
#define QXRDENABLERINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdEnableRingCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdEnableRingCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDENABLERINGCOMMAND_H
