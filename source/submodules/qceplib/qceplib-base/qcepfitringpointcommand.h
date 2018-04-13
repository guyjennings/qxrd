#ifndef QCEPFITRINGPOINTCOMMAND_H
#define QCEPFITRINGPOINTCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepFitRingPointCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepFitRingPointCommand(QcepPlotWidget            *plot,
                          QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPFITRINGPOINTCOMMAND_H
