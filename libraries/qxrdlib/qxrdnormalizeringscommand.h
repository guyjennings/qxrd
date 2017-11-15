#ifndef QXRDNORMALIZERINGSCOMMAND_H
#define QXRDNORMALIZERINGSCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdNormalizeRingsCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdNormalizeRingsCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDNORMALIZERINGSCOMMAND_H
