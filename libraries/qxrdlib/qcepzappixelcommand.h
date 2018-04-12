#ifndef QCEPZAPPIXELCOMMAND_H
#define QCEPZAPPIXELCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepZapPixelCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepZapPixelCommand(QcepPlotWidget            *plot,
                      QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPZAPPIXELCOMMAND_H
