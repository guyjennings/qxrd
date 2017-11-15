#ifndef QXRDFITPEAKCOMMAND_H
#define QXRDFITPEAKCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitPeakCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdFitPeakCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITPEAKCOMMAND_H
