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

  QxrdImagePlotWidgetSettingsPtr set(qSharedPointerDynamicCast<QxrdImagePlotWidgetSettings>(m_Settings));

  if (set) {
    for (int i=0; i<QxrdImagePlotWidget::LastScalingMode; i++) {
      QAction *act =
          scalingModes->addAction(QxrdImagePlotWidget::scalingModeName(i),
                                  [=]() { setDisplayScalingMode(i);});

      if (i == set->get_DisplayScalingMode()) {
        act->setCheckable(true);
        act->setChecked(true);
      }
    }
  }

//  scalingModes -> addAction("Percentage Scaling",
//                            [=]() { setDisplayScalingMode(QxrdImagePlotWidget::PercentageScaling); });

//  scalingModes -> addAction("Percentile Scaling",
//                            [=]() { setDisplayScalingMode(QxrdImagePlotWidget::PercentileScaling); });

//  scalingModes -> addAction("Absolute Scaling",
//                            [=]() { setDisplayScalingMode(QxrdImagePlotWidget::AbsoluteScaling); });

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
