#ifndef QXRDFITELLIPSECOMMAND_H
#define QXRDFITELLIPSECOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitEllipseCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdFitEllipseCommand(QString name, QxrdImagePlot* plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITELLIPSECOMMAND_H
