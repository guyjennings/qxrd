#ifndef QXRDSCALINGSUBMENUCOMMAND_H
#define QXRDSCALINGSUBMENUCOMMAND_H

#include "qxrdplotcontextmenucommand.h"
#include "qxrdimageplotwidget-ptr.h"
#include "qxrdimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdScalingSubmenuCommand : public QxrdPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdScalingSubmenuCommand(QxrdImagePlotWidget            *plot,
                            QxrdImagePlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);

  void setDisplayScalingMode(int n);
};

#endif // QXRDSCALINGSUBMENUCOMMAND_H
