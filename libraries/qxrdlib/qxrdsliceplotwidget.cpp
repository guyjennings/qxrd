#include "qxrdsliceplotwidget.h"
#include "qxrdsliceplotwidgetsettings.h"

QxrdSlicePlotWidget::QxrdSlicePlotWidget(QWidget *parent)
  : QxrdPlotWidget(parent)
{
}

void QxrdSlicePlotWidget::initialize(QxrdSlicePlotWidgetSettingsWPtr settings)
{
  QxrdPlotWidget::initialize(settings);
}
