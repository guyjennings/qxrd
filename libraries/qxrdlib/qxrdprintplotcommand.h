#ifndef QXRDPRINTPLOTCOMMAND_H
#define QXRDPRINTPLOTCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdPrintPlotCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdPrintPlotCommand(QString                    name,
                       QxrdPlotWidget            *plot,
                       QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDPRINTPLOTCOMMAND_H
