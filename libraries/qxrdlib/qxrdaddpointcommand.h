#ifndef QXRDADDPOINTCOMMAND_H
#define QXRDADDPOINTCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdAddPointCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdAddPointCommand(QString                    name,
                      QxrdPlotWidget            *plot,
                      QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADDPOINTCOMMAND_H
