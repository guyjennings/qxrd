#ifndef QXRDDISABLERINGCOMMAND_H
#define QXRDDISABLERINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdDisableRingCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdDisableRingCommand(QString name, QxrdImagePlot* plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDISABLERINGCOMMAND_H
