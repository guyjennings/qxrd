#include "qxrdintegratedplotwidget.h"
#include "qxrdintegratedplotwidgetsettings.h"

QxrdIntegratedPlotWidget::QxrdIntegratedPlotWidget(QWidget *parent) :
  QxrdPlotWidget(parent)
{
}

QxrdIntegratedPlotWidget::~QxrdIntegratedPlotWidget()
{
}

void QxrdIntegratedPlotWidget::initialize(QxrdIntegratedPlotWidgetSettingsWPtr settings)
{
  QxrdPlotWidget::initialize(settings);

  addPlotCommandSpacer();
}
