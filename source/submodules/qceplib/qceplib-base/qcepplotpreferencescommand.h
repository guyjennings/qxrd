#ifndef QCEPPLOTPREFERENCESCOMMAND_H
#define QCEPPLOTPREFERENCESCOMMAND_H

#include "qcepplotcontextmenucommand.h"

class QCEP_EXPORT QcepPlotPreferencesCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepPlotPreferencesCommand(QcepPlotWidget            *plot,
                             QcepPlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QCEPPLOTPREFERENCESCOMMAND_H
