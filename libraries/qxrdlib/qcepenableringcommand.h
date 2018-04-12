#ifndef QCEPENABLERINGCOMMAND_H
#define QCEPENABLERINGCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepEnableRingCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepEnableRingCommand(QcepPlotWidget            *plot,
                        QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPENABLERINGCOMMAND_H
