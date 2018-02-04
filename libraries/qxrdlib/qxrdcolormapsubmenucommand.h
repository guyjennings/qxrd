#ifndef QXRDCOLORMAPSUBMENUCOMMAND_H
#define QXRDCOLORMAPSUBMENUCOMMAND_H

#include "qxrdplotcontextmenucommand.h"
#include "qxrdimageplotwidget-ptr.h"
#include "qxrdimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdColorMapSubmenuCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdColorMapSubmenuCommand(QString                         name,
                             QxrdImagePlotWidget            *plot,
                             QxrdImagePlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);

  void setDisplayColorMap(int n);
};

#endif // QXRDCOLORMAPSUBMENUCOMMAND_H
