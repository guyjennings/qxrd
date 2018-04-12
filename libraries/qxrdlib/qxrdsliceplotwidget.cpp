#include "qxrdsliceplotwidget.h"
#include "qxrdsliceplotwidgetsettings.h"

QxrdSlicePlotWidget::QxrdSlicePlotWidget(QWidget *parent)
  : QcepPlotWidget(parent)
{
}

void QxrdSlicePlotWidget::initialize(QxrdSlicePlotWidgetSettingsWPtr settings)
{
  QcepPlotWidget::initialize(settings);
}
