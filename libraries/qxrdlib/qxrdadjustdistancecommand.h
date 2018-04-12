#ifndef QXRDADJUSTDISTANCECOMMAND_H
#define QXRDADJUSTDISTANCECOMMAND_H

#include "qcepplotcontextmenucommand.h"
#include "qxrdimageplot-ptr.h"

class QXRD_EXPORT QxrdAdjustDistanceCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdAdjustDistanceCommand(QcepPlotWidget            *plot,
                            QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADJUSTDISTANCECOMMAND_H
