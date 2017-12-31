#include "qxrdcenteringplotwidget.h"
#include "qxrdsetcentercommand.h"
#include "qxrdcontextseparatorcommand.h"
#include "qxrdenableringcommand.h"
#include "qxrdimageplot.h"
#include "qxrdcenteringplotwidgetsettings.h"

QxrdCenteringPlotWidget::QxrdCenteringPlotWidget(QWidget *parent) :
  QxrdPlotWidget(parent)
{
}

QxrdCenteringPlotWidget::~QxrdCenteringPlotWidget()
{
}

void QxrdCenteringPlotWidget::initialize(QxrdCenteringPlotWidgetSettingsWPtr settings)
{
  QxrdPlotWidget::initialize(settings);

  addPlotCommand(QxrdPlotCommandPtr(new QxrdSetCenterCommand("Set Center", this, settings)));
  addPlotCommandSpacer();

  addPlotCommand(QxrdPlotCommandPtr(new QxrdContextSeparatorCommand("Separator", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdEnableRingCommand("Enable Ring", this, settings)));
}
