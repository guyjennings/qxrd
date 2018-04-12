#ifndef QXRDNORMALIZERINGSCOMMAND_H
#define QXRDNORMALIZERINGSCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdNormalizeRingsCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdNormalizeRingsCommand(QString                    name,
                            QcepPlotWidget            *plot,
                            QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDNORMALIZERINGSCOMMAND_H
