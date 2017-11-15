#ifndef QXRDADDPOINTCOMMAND_H
#define QXRDADDPOINTCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdAddPointCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdAddPointCommand(QString name, QxrdImagePlot* plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADDPOINTCOMMAND_H
