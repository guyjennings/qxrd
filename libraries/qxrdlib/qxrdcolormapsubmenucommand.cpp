#include "qxrdcolormapsubmenucommand.h"
#include <QMenu>
#include "qxrdcolormaplibrary.h"
#include "qxrdimageplotwidget.h"
#include "qxrdimageplotwidgetsettings.h"

QxrdColorMapSubmenuCommand::QxrdColorMapSubmenuCommand(QString name, QxrdImagePlotWidget *plot, QxrdImagePlotWidgetSettingsWPtr set)
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
      QAction *act = colorMaps -> addAction(QxrdColorMapLibrary::colorMapName(i),
                                            [=]() { setDisplayColorMap(i); });

      if (i == set->get_DisplayColorMap()) {
        act->setCheckable(true);
        act->setChecked(true);
      }
    }
  }

  colorMap ->setMenu(colorMaps);

  return colorMap;
}

void QxrdColorMapSubmenuCommand::setDisplayColorMap(int n)
{
  QxrdImagePlotWidgetSettingsPtr set(qSharedPointerDynamicCast<QxrdImagePlotWidgetSettings>(m_Settings));

  if (set) {
    set -> set_DisplayColorMap(n);
  }
}
