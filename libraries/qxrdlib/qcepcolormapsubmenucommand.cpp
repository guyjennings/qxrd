#include "qcepcolormapsubmenucommand.h"
#include <QMenu>
#include "qcepcolormaplibrary.h"
#include "qcepimageplotwidget.h"
#include "qcepimageplotwidgetsettings.h"

QcepColorMapSubmenuCommand::QcepColorMapSubmenuCommand(QString name, QcepImagePlotWidget *plot, QcepImagePlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("colorMapSubmenu", "Color Map", plot, set)
{
}

QAction* QcepColorMapSubmenuCommand::contextMenuAction(const QPoint & /*pos*/)
{
  QAction *colorMap = newContextAction("Color Map", NULL, NULL);

  QMenu *colorMaps = new QMenu("Color Maps");

  QcepImagePlotWidgetSettingsPtr set(qSharedPointerDynamicCast<QcepImagePlotWidgetSettings>(m_Settings));

  if (set) {
    for (int i=0; i<QcepColorMapLibrary::colorMapCount(); i++) {
      QAction *act = colorMaps -> addAction(QcepColorMapLibrary::colorMapName(i),
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

void QcepColorMapSubmenuCommand::setDisplayColorMap(int n)
{
  QcepImagePlotWidgetSettingsPtr set(qSharedPointerDynamicCast<QcepImagePlotWidgetSettings>(m_Settings));

  if (set) {
    set -> set_DisplayColorMap(n);
  }
}
