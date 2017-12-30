#include "qxrddistortionplotwidget.h"
#include "qxrddistortionplotwidgetsettings.h"

QxrdDistortionPlotWidget::QxrdDistortionPlotWidget(QWidget *parent)
  : QxrdPlotWidget(parent)
{
}

void QxrdDistortionPlotWidget::initialize(QxrdDistortionPlotWidgetSettingsWPtr settings)
{
  QxrdPlotWidget::initialize(settings);
}
