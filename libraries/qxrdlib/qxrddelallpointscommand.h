#ifndef QXRDDELALLPOINTSCOMMAND_H
#define QXRDDELALLPOINTSCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdDelAllPointsCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdDelAllPointsCommand(QcepPlotWidget            *plot,
                          QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDELALLPOINTSCOMMAND_H
