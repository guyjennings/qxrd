#ifndef QXRDFITCIRCLECOMMAND_H
#define QXRDFITCIRCLECOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitCircleCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdFitCircleCommand(QString                    name,
                       QxrdPlotWidget            *plot,
                       QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITCIRCLECOMMAND_H
