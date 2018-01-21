#ifndef QXRDADJUSTFITCOMMAND_H
#define QXRDADJUSTFITCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdAdjustFitCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdAdjustFitCommand(QString                    name,
                       QxrdPlotWidget            *plot,
                       QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADJUSTFITCOMMAND_H