#include "qxrdextrainputsplotwidget.h"
#include "qxrdextrainputsplotwidgetsettings.h"

QxrdExtraInputsPlotWidget::QxrdExtraInputsPlotWidget(QWidget *parent)
  : QxrdPlotWidget(parent)
{
}

void QxrdExtraInputsPlotWidget::initialize(QxrdExtraInputsPlotWidgetSettingsWPtr settings)
{
  QxrdPlotWidget::initialize(settings);
}
