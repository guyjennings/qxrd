#ifndef QXRDPRINTPLOTCOMMAND_H
#define QXRDPRINTPLOTCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QcepPrintPlotCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepPrintPlotCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDPRINTPLOTCOMMAND_H
