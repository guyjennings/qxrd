#include "qxrdhistogramplotwidget.h"
#include "qxrdhistogramplotwidgetsettings.h"

QxrdHistogramPlotWidget::QxrdHistogramPlotWidget(QWidget *parent)
  : QxrdPlotWidget(parent)
{

}

void QxrdHistogramPlotWidget::initialize(QxrdHistogramPlotWidgetSettingsWPtr settings)
{
  QxrdPlotWidget::initialize(settings);
}
