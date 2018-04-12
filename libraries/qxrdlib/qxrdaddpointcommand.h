#ifndef QXRDADDPOINTCOMMAND_H
#define QXRDADDPOINTCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdAddPointCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdAddPointCommand(QcepPlotWidget            *plot,
                      QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADDPOINTCOMMAND_H
