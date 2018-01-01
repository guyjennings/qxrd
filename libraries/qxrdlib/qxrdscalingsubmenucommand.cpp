#include "qxrdscalingsubmenucommand.h"
#include "qxrdplotwidgetsettings.h"
#include <QMenu>

QxrdScalingSubmenuCommand::QxrdScalingSubmenuCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotContextMenuCommand(name, plot, set)
{
}

QAction* QxrdScalingSubmenuCommand::contextMenuAction(const QPoint & /*pos*/)
{
  QAction *scalingMode = newContextAction("Scaling Mode", NULL, NULL);

  QMenu   *scalingModes = new QMenu("Scaling Mode");

  scalingModes -> addAction("Percentage Scaling", m_Plot, &QxrdImagePlot::setPercentageScaling);
  scalingModes -> addAction("Percentile Scaling", m_Plot, &QxrdImagePlot::setPercentileScaling);
  scalingModes -> addAction("Absolute Scaling",   m_Plot, &QxrdImagePlot::setAbsoluteScaling);

  scalingMode ->setMenu(scalingModes);

  return scalingMode;
}
