#include "qxrdcenteringplotwidget.h"
#include "qxrdsetcentercommand.h"
#include "qxrdcontextseparatorcommand.h"
#include "qxrdenableringcommand.h"
#include "qxrdimageplot.h"
#include "qxrdcenteringplotwidgetsettings.h"

QxrdCenteringPlotWidget::QxrdCenteringPlotWidget(QWidget *parent) :
  QxrdPlotWidget(parent),
  m_CenterFinder()
{
}

QxrdCenteringPlotWidget::~QxrdCenteringPlotWidget()
{
}

void QxrdCenteringPlotWidget::initialize(QxrdCenteringPlotWidgetSettingsWPtr settings,
                                         QxrdCenterFinderWPtr cf)
{
  QxrdPlotWidget::initialize(settings);
  m_CenterFinder = cf;

  QxrdCenterFinderPtr c(m_CenterFinder);

  if (c) {
    addPlotCommand(QxrdPlotCommandPtr(new QxrdSetCenterCommand(this, settings, c)));
  }

  addPlotCommandSpacer();

  addPlotCommand(QxrdPlotCommandPtr(new QxrdContextSeparatorCommand(this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdEnableRingCommand(this, settings)));
}
