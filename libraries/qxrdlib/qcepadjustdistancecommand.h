#ifndef QCEPADJUSTDISTANCECOMMAND_H
#define QCEPADJUSTDISTANCECOMMAND_H

#include "qcepplotcontextmenucommand.h"
#include "qcepimageplot-ptr.h"

class QCEP_EXPORT QcepAdjustDistanceCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepAdjustDistanceCommand(QcepPlotWidget            *plot,
                            QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPADJUSTDISTANCECOMMAND_H
