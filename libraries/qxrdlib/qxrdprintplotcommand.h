#ifndef QXRDPRINTPLOTCOMMAND_H
#define QXRDPRINTPLOTCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdPrintPlotCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdPrintPlotCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDPRINTPLOTCOMMAND_H
