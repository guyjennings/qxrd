#ifndef QXRDFITPEAKCOMMAND_H
#define QXRDFITPEAKCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitPeakCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdFitPeakCommand(QcepPlotWidget            *plot,
                     QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITPEAKCOMMAND_H
