#include "qxrdcolormapsubmenucommand.h"
#include "qxrdplotwidgetsettings.h"
#include <QMenu>

QxrdColorMapSubmenuCommand::QxrdColorMapSubmenuCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{
}

QAction* QxrdColorMapSubmenuCommand::contextMenuAction(const QPoint & /*pos*/)
{
  QAction *colorMap = newContextAction("Color Map", NULL, NULL);

  QMenu *colorMaps = new QMenu("Color Maps");

  colorMaps -> addAction("Grayscale",          m_Plot, &QxrdImagePlot::setGrayscale);
  colorMaps -> addAction("Inverse Grayscale",  m_Plot, &QxrdImagePlot::setInverseGrayscale);
  colorMaps -> addAction("Earth Tones",        m_Plot, &QxrdImagePlot::setEarthTones);
  colorMaps -> addAction("Spectrum",           m_Plot, &QxrdImagePlot::setSpectrum);
  colorMaps -> addAction("Fire",               m_Plot, &QxrdImagePlot::setFire);
  colorMaps -> addAction("Ice",                m_Plot, &QxrdImagePlot::setIce);

  QxrdPlotWidgetSettingsPtr set(m_Settings);

  if (set) {
  }

  colorMap ->setMenu(colorMaps);

  return colorMap;
}
