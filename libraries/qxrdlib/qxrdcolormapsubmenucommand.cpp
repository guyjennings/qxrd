#include "qxrdcolormapsubmenucommand.h"
#include "qxrdplotwidgetsettings.h"
#include <QMenu>
#include "qxrdcolormaplibrary.h"
#include "qxrdimageplotwidgetsettings.h"

QxrdColorMapSubmenuCommand::QxrdColorMapSubmenuCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{
}

QAction* QxrdColorMapSubmenuCommand::contextMenuAction(const QPoint & /*pos*/)
{
  QAction *colorMap = newContextAction("Color Map", NULL, NULL);

  QMenu *colorMaps = new QMenu("Color Maps");

  QxrdImagePlotWidgetSettingsPtr set(qSharedPointerDynamicCast<QxrdImagePlotWidgetSettings>(m_Settings));

  if (set) {
    for (int i=0; i<QxrdColorMapLibrary::colorMapCount(); i++) {
      colorMaps -> addAction(QxrdColorMapLibrary::colorMapName(i),
                             [=]() { set->set_DisplayColorMap(i); });
    }
  }

  colorMap ->setMenu(colorMaps);

  return colorMap;
}
