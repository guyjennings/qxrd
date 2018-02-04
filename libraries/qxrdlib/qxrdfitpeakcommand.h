#ifndef QXRDFITPEAKCOMMAND_H
#define QXRDFITPEAKCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdFitPeakCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdFitPeakCommand(QString                    name,
                     QxrdPlotWidget            *plot,
                     QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDFITPEAKCOMMAND_H
