#ifndef QCEPDELRINGCOMMAND_H
#define QCEPDELRINGCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepDelRingCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepDelRingCommand(QcepPlotWidget            *plot,
                     QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPDELRINGCOMMAND_H
