#ifndef QXRDDELRINGCOMMAND_H
#define QXRDDELRINGCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdDelRingCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdDelRingCommand(QString                    name,
                     QxrdPlotWidget            *plot,
                     QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDELRINGCOMMAND_H
