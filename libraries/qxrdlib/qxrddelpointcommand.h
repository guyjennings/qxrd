#ifndef QXRDDELPOINTCOMMAND_H
#define QXRDDELPOINTCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdDelPointCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdDelPointCommand(QcepPlotWidget            *plot,
                      QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDELPOINTCOMMAND_H
