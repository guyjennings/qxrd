#ifndef QXRDPLOTPREFERENCESCOMMAND_H
#define QXRDPLOTPREFERENCESCOMMAND_H

#include "qxrdplotcontextmenucommand.h"

class QXRD_EXPORT QxrdPlotPreferencesCommand : public QxrdPlotContextMenuCommand
{
public:
  QxrdPlotPreferencesCommand(QString                    name,
                             QxrdPlotWidget            *plot,
                             QxrdPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDPLOTPREFERENCESCOMMAND_H
