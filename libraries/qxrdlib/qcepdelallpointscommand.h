#ifndef QCEPDELALLPOINTSCOMMAND_H
#define QCEPDELALLPOINTSCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepDelAllPointsCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepDelAllPointsCommand(QcepPlotWidget            *plot,
                          QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPDELALLPOINTSCOMMAND_H
