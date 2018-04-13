#ifndef QCEPDELPOINTCOMMAND_H
#define QCEPDELPOINTCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepDelPointCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepDelPointCommand(QcepPlotWidget            *plot,
                      QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPDELPOINTCOMMAND_H
