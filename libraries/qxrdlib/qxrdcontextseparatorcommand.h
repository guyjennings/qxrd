#ifndef QXRDCONTEXTSEPARATORCOMMAND_H
#define QXRDCONTEXTSEPARATORCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QxrdContextSeparatorCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdContextSeparatorCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
  bool     contextMenuSeparator();
};

#endif // QXRDCONTEXTSEPARATORCOMMAND_H
