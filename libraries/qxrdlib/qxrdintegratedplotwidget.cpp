#include "qxrdintegratedplotwidget.h"
#include "qxrdintegratedplotwidgetsettings.h"

QxrdIntegratedPlotWidget::QxrdIntegratedPlotWidget(QWidget *parent) :
  QcepPlotWidget(parent)
{
}

QxrdIntegratedPlotWidget::~QxrdIntegratedPlotWidget()
{
}

void QxrdIntegratedPlotWidget::initialize(QxrdIntegratedPlotWidgetSettingsWPtr settings)
{
  QcepPlotWidget::initialize(settings);

  addPlotCommandSpacer();
}
