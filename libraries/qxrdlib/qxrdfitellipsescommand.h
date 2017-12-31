#ifndef QXRDFITELLIPSESCOMMAND_H
#define QXRDFITELLIPSESCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitEllipsesCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdFitEllipsesCommand(QString                    name,
                         QxrdPlotWidget            *plot,
                         QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITELLIPSESCOMMAND_H
