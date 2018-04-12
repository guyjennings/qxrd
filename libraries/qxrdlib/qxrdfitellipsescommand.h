#ifndef QXRDFITELLIPSESCOMMAND_H
#define QXRDFITELLIPSESCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitEllipsesCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdFitEllipsesCommand(QcepPlotWidget            *plot,
                         QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITELLIPSESCOMMAND_H
