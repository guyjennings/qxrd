#ifndef QCEPNORMALIZERINGSCOMMAND_H
#define QCEPNORMALIZERINGSCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepNormalizeRingsCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepNormalizeRingsCommand(QString                    name,
                            QcepPlotWidget            *plot,
                            QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPNORMALIZERINGSCOMMAND_H
