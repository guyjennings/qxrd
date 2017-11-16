#include "qxrddisplaysubmenucommand.h"
#include <QMenu>

QxrdDisplaySubmenuCommand::QxrdDisplaySubmenuCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotContextMenuCommand(name, plot)
{
}

QAction* QxrdDisplaySubmenuCommand::contextMenuAction(const QPoint &pos)
{
  QAction *displaySubmenu = newContextAction("Display", NULL, NULL);

  QMenu   *displayMenu = new QMenu("Display");

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
