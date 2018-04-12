#ifndef QCEPCOLORMAPSUBMENUCOMMAND_H
#define QCEPCOLORMAPSUBMENUCOMMAND_H

#include "qcepplotcontextmenucommand.h"
#include "qcepimageplotwidget-ptr.h"
#include "qcepimageplotwidgetsettings-ptr.h"

class QCEP_EXPORT QcepColorMapSubmenuCommand : public QcepPlotContextMenuCommand
{
  Q_OBJECT

public:
  QcepColorMapSubmenuCommand(QString                         name,
                             QcepImagePlotWidget            *plot,
                             QcepImagePlotWidgetSettingsWPtr set);

  QAction *contextMenuAction(const QPoint &pos);

  void setDisplayColorMap(int n);
};

#endif // QCEPCOLORMAPSUBMENUCOMMAND_H
