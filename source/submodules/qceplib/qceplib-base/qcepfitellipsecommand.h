#ifndef QCEPFITELLIPSECOMMAND_H
#define QCEPFITELLIPSECOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepFitEllipseCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepFitEllipseCommand(QcepPlotWidget            *plot,
                        QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPFITELLIPSECOMMAND_H
