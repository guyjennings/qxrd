#include "qxrddisplaysubmenucommand.h"
#include "qxrdplotwidgetsettings.h"
#include <QMenu>

QxrdDisplaySubmenuCommand::QxrdDisplaySubmenuCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{
}

QAction* QxrdDisplaySubmenuCommand::contextMenuAction(const QPoint & /*pos*/)
{
  QAction *displaySubmenu = newContextAction("Display", NULL, NULL);

  QMenu   *displayMenu = new QMenu("Display");

  QMenu   *logAxesMenu = new QMenu("Axes");

  QxrdPlotWidgetSettingsPtr set(m_Settings);

  if (set) {
    QAction *xLog = logAxesMenu -> addAction("X axis log", set.data(), &QxrdPlotWidgetSettings::toggle_XAxisLog);
    QAction *yLog = logAxesMenu -> addAction("Y axis log", set.data(), &QxrdPlotWidgetSettings::toggle_YAxisLog);
    QAction *x2Log = logAxesMenu -> addAction("X2 axis log", set.data(), &QxrdPlotWidgetSettings::toggle_X2AxisLog);
    QAction *y2Log = logAxesMenu -> addAction("Y2 axis log", set.data(), &QxrdPlotWidgetSettings::toggle_Y2AxisLog);

    xLog -> setCheckable(true);
    yLog -> setCheckable(true);
    x2Log -> setCheckable(true);
    y2Log -> setCheckable(true);

    xLog -> setChecked(set->get_XAxisLog());
    yLog -> setChecked(set->get_YAxisLog());
    x2Log -> setChecked(set->get_X2AxisLog());
    y2Log -> setChecked(set->get_Y2AxisLog());
  }

  displayMenu -> addMenu(logAxesMenu);

  QMenu   *scalingModes = new QMenu("Scaling Mode");

  scalingModes -> addAction("Percentage Scaling", m_Plot, &QxrdImagePlot::setPercentageScaling);
  scalingModes -> addAction("Percentile Scaling", m_Plot, &QxrdImagePlot::setPercentileScaling);
  scalingModes -> addAction("Absolute Scaling",   m_Plot, &QxrdImagePlot::setAbsoluteScaling);

  displayMenu -> addMenu(scalingModes);

  QMenu *colorMaps = new QMenu("Color Maps");

  colorMaps -> addAction("Grayscale",          m_Plot, &QxrdImagePlot::setGrayscale);
  colorMaps -> addAction("Inverse Grayscale",  m_Plot, &QxrdImagePlot::setInverseGrayscale);
  colorMaps -> addAction("Earth Tones",        m_Plot, &QxrdImagePlot::setEarthTones);
  colorMaps -> addAction("Spectrum",           m_Plot, &QxrdImagePlot::setSpectrum);
  colorMaps -> addAction("Fire",               m_Plot, &QxrdImagePlot::setFire);
  colorMaps -> addAction("Ice",                m_Plot, &QxrdImagePlot::setIce);

  displayMenu -> addMenu(colorMaps);

  displaySubmenu ->setMenu(displayMenu);

  return displaySubmenu;
}
