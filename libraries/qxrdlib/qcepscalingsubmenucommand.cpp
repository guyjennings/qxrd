#include "qcepscalingsubmenucommand.h"
#include "qcepimageplotwidget.h"
#include "qcepimageplotwidgetsettings.h"
#include <QMenu>

QcepScalingSubmenuCommand::QcepScalingSubmenuCommand(QcepImagePlotWidget *plot, QcepImagePlotWidgetSettingsWPtr set)
  : QcepPlotContextMenuCommand("displayMenu", "Display", plot, set)
{
}

QAction* QcepScalingSubmenuCommand::contextMenuAction(const QPoint & /*pos*/)
{
  QAction *scalingMode = newContextAction("Scaling Mode", NULL, NULL);

  QMenu   *scalingModes = new QMenu("Scaling Mode");

  QcepImagePlotWidgetSettingsPtr set(qSharedPointerDynamicCast<QcepImagePlotWidgetSettings>(m_Settings));

  if (set) {
    for (int i=0; i<QcepImagePlotWidget::LastScalingMode; i++) {
      QAction *act =
          scalingModes->addAction(QcepImagePlotWidget::scalingModeName(i),
                                  [=]() { setDisplayScalingMode(i);});

      if (i == set->get_DisplayScalingMode()) {
        act->setCheckable(true);
        act->setChecked(true);
      }
    }
  }

//  scalingModes -> addAction("Percentage Scaling",
//                            [=]() { setDisplayScalingMode(QcepImagePlotWidget::PercentageScaling); });

//  scalingModes -> addAction("Percentile Scaling",
//                            [=]() { setDisplayScalingMode(QcepImagePlotWidget::PercentileScaling); });

//  scalingModes -> addAction("Absolute Scaling",
//                            [=]() { setDisplayScalingMode(QcepImagePlotWidget::AbsoluteScaling); });

  scalingMode ->setMenu(scalingModes);

  return scalingMode;
}

void QcepScalingSubmenuCommand::setDisplayScalingMode(int n)
{
  QcepImagePlotWidgetSettingsPtr set(qSharedPointerDynamicCast<QcepImagePlotWidgetSettings>(m_Settings));

  if (set) {
    set->set_DisplayScalingMode(n);
  }
}
