#ifndef QCEPFITELLIPSESCOMMAND_H
#define QCEPFITELLIPSESCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepFitEllipsesCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepFitEllipsesCommand(QcepPlotWidget            *plot,
                         QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPFITELLIPSESCOMMAND_H
