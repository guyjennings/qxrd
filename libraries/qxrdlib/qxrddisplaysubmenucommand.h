#ifndef QXRDDISPLAYSUBMENUCOMMAND_H
#define QXRDDISPLAYSUBMENUCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdDisplaySubmenuCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdDisplaySubmenuCommand(QString name, QxrdPlotWidget *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDISPLAYSUBMENUCOMMAND_H
