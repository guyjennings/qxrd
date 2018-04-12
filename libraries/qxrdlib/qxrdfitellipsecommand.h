#ifndef QXRDFITELLIPSECOMMAND_H
#define QXRDFITELLIPSECOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitEllipseCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdFitEllipseCommand(QcepPlotWidget            *plot,
                        QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITELLIPSECOMMAND_H
