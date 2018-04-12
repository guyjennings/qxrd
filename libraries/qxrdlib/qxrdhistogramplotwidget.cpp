#include "qxrdhistogramplotwidget.h"
#include "qxrdhistogramplotwidgetsettings.h"

QxrdHistogramPlotWidget::QxrdHistogramPlotWidget(QWidget *parent)
  : QcepPlotWidget(parent)
{

}

void QxrdHistogramPlotWidget::initialize(QxrdHistogramPlotWidgetSettingsWPtr settings)
{
  QcepPlotWidget::initialize(settings);
}
