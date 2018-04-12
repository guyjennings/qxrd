#include "qxrddistortionplotwidget.h"
#include "qxrddistortionplotwidgetsettings.h"

QxrdDistortionPlotWidget::QxrdDistortionPlotWidget(QWidget *parent)
  : QcepPlotWidget(parent)
{
}

void QxrdDistortionPlotWidget::initialize(QxrdDistortionPlotWidgetSettingsWPtr settings)
{
  QcepPlotWidget::initialize(settings);

  addPlotCommandSpacer();
}
