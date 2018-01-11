#include "qxrdscalingsubmenucommand.h"
#include "qxrdimageplotwidget.h"
#include "qxrdimageplotwidgetsettings.h"
#include <QMenu>

QxrdScalingSubmenuCommand::QxrdScalingSubmenuCommand(QString name, QxrdImagePlotWidget *plot, QxrdImagePlotWidgetSettingsWPtr set)
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

void QxrdScalingSubmenuCommand::setDisplayScalingMode(int n)
{
  QxrdImagePlotWidgetSettingsPtr set(qSharedPointerDynamicCast<QxrdImagePlotWidgetSettings>(m_Settings));

  if (set) {
    set->set_DisplayScalingMode(n);
  }
}
