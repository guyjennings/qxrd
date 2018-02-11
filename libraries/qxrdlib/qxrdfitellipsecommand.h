#ifndef QXRDFITELLIPSECOMMAND_H
#define QXRDFITELLIPSECOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitEllipseCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdFitEllipseCommand(QxrdPlotWidget            *plot,
                        QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITELLIPSECOMMAND_H
