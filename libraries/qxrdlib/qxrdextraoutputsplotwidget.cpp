#include "qxrdextraoutputsplotwidget.h"
#include "qxrdextraoutputsplotwidgetsettings.h"

QxrdExtraOutputsPlotWidget::QxrdExtraOutputsPlotWidget(QWidget *parent)
  : QxrdPlotWidget(parent)
{
}

void QxrdExtraOutputsPlotWidget::initialize(QxrdExtraOutputsPlotWidgetSettingsWPtr settings)
{
  QxrdPlotWidget::initialize(settings);

  addPlotCommandSpacer();
}
