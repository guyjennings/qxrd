#include "qxrddetectorplotwidget.h"
#include "qxrddetectorplotwidgetsettings.h"

QxrdDetectorPlotWidget::QxrdDetectorPlotWidget(QWidget *parent)
  : QxrdPlotWidget(parent)
{

}

void QxrdDetectorPlotWidget::initialize(QxrdDetectorPlotWidgetSettingsWPtr settings)
{
  QxrdPlotWidget::initialize(settings);

  addPlotCommandSpacer();
}
