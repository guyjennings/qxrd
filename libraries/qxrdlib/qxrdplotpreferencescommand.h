#ifndef QXRDPLOTPREFERENCESCOMMAND_H
#define QXRDPLOTPREFERENCESCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdPlotPreferencesCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdPlotPreferencesCommand(QxrdPlotWidget            *plot,
                             QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDPLOTPREFERENCESCOMMAND_H
