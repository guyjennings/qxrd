#ifndef QXRDFITRINGPOINTCOMMAND_H
#define QXRDFITRINGPOINTCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitRingPointCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdFitRingPointCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITRINGPOINTCOMMAND_H
