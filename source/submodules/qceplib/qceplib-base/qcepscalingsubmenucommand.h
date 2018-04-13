#ifndef QCEPSCALINGSUBMENUCOMMAND_H
#define QCEPSCALINGSUBMENUCOMMAND_H

#include "qcepplotcontextmenucommand.h"
#include "qcepimageplotwidget-ptr.h"
#include "qcepimageplotwidgetsettings-ptr.h"

class QCEP_EXPORT QcepScalingSubmenuCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepScalingSubmenuCommand(QcepImagePlotWidget            *plot,
                            QcepImagePlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);

  void setDisplayScalingMode(int n);
};

#endif // QCEPSCALINGSUBMENUCOMMAND_H
