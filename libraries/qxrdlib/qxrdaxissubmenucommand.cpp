#include "qxrdaxissubmenucommand.h"
#include "qxrdplotwidgetsettings.h"
#include <QMenu>

QxrdAxisSubmenuCommand::QxrdAxisSubmenuCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{
}

QAction* QxrdAxisSubmenuCommand::contextMenuAction(const QPoint & /*pos*/)
{
  QAction *displaySubmenu = newContextAction("Axes", NULL, NULL);

  QMenu   *displayMenu = new QMenu("Axes");

  QxrdPlotWidgetSettingsPtr set(m_Settings);

  if (set) {
    QAction *xLog = displayMenu -> addAction("X axis log", set.data(), &QxrdPlotWidgetSettings::toggle_XAxisLog);
    QAction *yLog = displayMenu -> addAction("Y axis log", set.data(), &QxrdPlotWidgetSettings::toggle_YAxisLog);
    QAction *x2Log = displayMenu -> addAction("X2 axis log", set.data(), &QxrdPlotWidgetSettings::toggle_X2AxisLog);
    QAction *y2Log = displayMenu -> addAction("Y2 axis log", set.data(), &QxrdPlotWidgetSettings::toggle_Y2AxisLog);

    xLog -> setCheckable(true);
    yLog -> setCheckable(true);
    x2Log -> setCheckable(true);
    y2Log -> setCheckable(true);

    xLog -> setChecked(set->get_XAxisLog());
    yLog -> setChecked(set->get_YAxisLog());
    x2Log -> setChecked(set->get_X2AxisLog());
    y2Log -> setChecked(set->get_Y2AxisLog());
  }

  displaySubmenu ->setMenu(displayMenu);

  return displaySubmenu;
}
