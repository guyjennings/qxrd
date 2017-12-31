#ifndef QXRDDELALLPOINTSCOMMAND_H
#define QXRDDELALLPOINTSCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdDelAllPointsCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdDelAllPointsCommand(QString                    name,
                          QxrdPlotWidget            *plot,
                          QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDDELALLPOINTSCOMMAND_H
