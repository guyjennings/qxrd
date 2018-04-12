#ifndef QCEPADDPOINTCOMMAND_H
#define QCEPADDPOINTCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepAddPointCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepAddPointCommand(QcepPlotWidget            *plot,
                      QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPADDPOINTCOMMAND_H
