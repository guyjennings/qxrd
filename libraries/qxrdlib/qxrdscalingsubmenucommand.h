#ifndef QXRDSCALINGSUBMENUCOMMAND_H
#define QXRDSCALINGSUBMENUCOMMAND_H

#include "qcepplotcontextmenucommand.h"
#include "qcepimageplotwidget-ptr.h"
#include "qcepimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdScalingSubmenuCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QxrdScalingSubmenuCommand(QcepImagePlotWidget            *plot,
                            QcepImagePlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);

  void setDisplayScalingMode(int n);
};

#endif // QXRDSCALINGSUBMENUCOMMAND_H
