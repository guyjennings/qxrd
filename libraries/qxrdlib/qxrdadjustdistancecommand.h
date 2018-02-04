#ifndef QXRDADJUSTDISTANCECOMMAND_H
#define QXRDADJUSTDISTANCECOMMAND_H

#include "qxrdplotcontextmenucommand.h"
#include "qxrdimageplot-ptr.h"

class QXRD_EXPORT QxrdAdjustDistanceCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdAdjustDistanceCommand(QString                    name,
                            QxrdPlotWidget            *plot,
                            QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDADJUSTDISTANCECOMMAND_H
