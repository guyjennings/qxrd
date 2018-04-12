#ifndef QXRDADJUSTFITCOMMAND_H
#define QXRDADJUSTFITCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdAdjustFitCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdAdjustFitCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADJUSTFITCOMMAND_H
