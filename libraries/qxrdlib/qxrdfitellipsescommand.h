#ifndef QXRDFITELLIPSESCOMMAND_H
#define QXRDFITELLIPSESCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdFitEllipsesCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdFitEllipsesCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITELLIPSESCOMMAND_H
