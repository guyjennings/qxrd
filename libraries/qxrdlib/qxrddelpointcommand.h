#ifndef QXRDDELPOINTCOMMAND_H
#define QXRDDELPOINTCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdDelPointCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdDelPointCommand(QString                    name,
                      QxrdPlotWidget            *plot,
                      QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDELPOINTCOMMAND_H
