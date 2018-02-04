#ifndef QXRDCONTEXTSEPARATORCOMMAND_H
#define QXRDCONTEXTSEPARATORCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdContextSeparatorCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdContextSeparatorCommand(QString                    name,
                              QxrdPlotWidget            *plot,
                              QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDCONTEXTSEPARATORCOMMAND_H
