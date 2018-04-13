#ifndef QCEPFITPEAKCOMMAND_H
#define QCEPFITPEAKCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepFitPeakCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepFitPeakCommand(QcepPlotWidget            *plot,
                     QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPFITPEAKCOMMAND_H
