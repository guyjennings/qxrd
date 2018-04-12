#ifndef QXRDCOLORMAPSUBMENUCOMMAND_H
#define QXRDCOLORMAPSUBMENUCOMMAND_H

#include "qcepplotcontextmenucommand.h"
#include "qcepimageplotwidget-ptr.h"
#include "qcepimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdColorMapSubmenuCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdColorMapSubmenuCommand(QString                         name,
                             QcepImagePlotWidget            *plot,
                             QcepImagePlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);

  void setDisplayColorMap(int n);
};

#endif // QXRDCOLORMAPSUBMENUCOMMAND_H
