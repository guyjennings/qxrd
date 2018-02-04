#ifndef QXRDFITRINGPOINTCOMMAND_H
#define QXRDFITRINGPOINTCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitRingPointCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdFitRingPointCommand(QString                    name,
                          QxrdPlotWidget            *plot,
                          QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITRINGPOINTCOMMAND_H
