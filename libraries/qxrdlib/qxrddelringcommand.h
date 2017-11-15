#ifndef QXRDDELRINGCOMMAND_H
#define QXRDDELRINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdDelRingCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdDelRingCommand(QString name, QxrdImagePlot* plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDELRINGCOMMAND_H
