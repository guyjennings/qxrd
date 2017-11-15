#ifndef QXRDFITPEAKCOMMAND_H
#define QXRDFITPEAKCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdFitPeakCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdFitPeakCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITPEAKCOMMAND_H
