#ifndef QCEPADJUSTFITCOMMAND_H
#define QCEPADJUSTFITCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepAdjustFitCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepAdjustFitCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPADJUSTFITCOMMAND_H
