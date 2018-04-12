#ifndef QCEPPRINTPLOTCOMMAND_H
#define QCEPPRINTPLOTCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepPrintPlotCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepPrintPlotCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPPRINTPLOTCOMMAND_H
