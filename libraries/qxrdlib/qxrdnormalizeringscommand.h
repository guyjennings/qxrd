#ifndef QXRDNORMALIZERINGSCOMMAND_H
#define QXRDNORMALIZERINGSCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdNormalizeRingsCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdNormalizeRingsCommand(QString                    name,
                            QxrdPlotWidget            *plot,
                            QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDNORMALIZERINGSCOMMAND_H
