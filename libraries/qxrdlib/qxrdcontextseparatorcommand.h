#ifndef QXRDCONTEXTSEPARATORCOMMAND_H
#define QXRDCONTEXTSEPARATORCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QXRD_EXPORT QxrdContextSeparatorCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdContextSeparatorCommand(QcepPlotWidget            *plot,
                              QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDCONTEXTSEPARATORCOMMAND_H
