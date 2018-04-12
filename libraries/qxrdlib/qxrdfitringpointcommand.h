#ifndef QXRDFITRINGPOINTCOMMAND_H
#define QXRDFITRINGPOINTCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitRingPointCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdFitRingPointCommand(QcepPlotWidget            *plot,
                          QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITRINGPOINTCOMMAND_H
