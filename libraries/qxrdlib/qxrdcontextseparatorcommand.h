#ifndef QXRDCONTEXTSEPARATORCOMMAND_H
#define QXRDCONTEXTSEPARATORCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdContextSeparatorCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdContextSeparatorCommand(QString name, QxrdPlotWidget *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDCONTEXTSEPARATORCOMMAND_H
