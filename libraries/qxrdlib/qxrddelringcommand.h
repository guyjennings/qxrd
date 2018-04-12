#ifndef QXRDDELRINGCOMMAND_H
#define QXRDDELRINGCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdDelRingCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdDelRingCommand(QcepPlotWidget            *plot,
                     QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDELRINGCOMMAND_H
