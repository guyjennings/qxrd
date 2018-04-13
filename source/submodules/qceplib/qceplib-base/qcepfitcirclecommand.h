#ifndef QCEPFITCIRCLECOMMAND_H
#define QCEPFITCIRCLECOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepFitCircleCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepFitCircleCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPFITCIRCLECOMMAND_H
