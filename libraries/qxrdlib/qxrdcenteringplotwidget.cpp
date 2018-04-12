#include "qxrdcenteringplotwidget.h"
#include "qxrdsetcentercommand.h"
#include "qxrdcontextseparatorcommand.h"
#include "qxrdenableringcommand.h"
#include "qcepimageplot.h"
#include "qxrdcenteringplotwidgetsettings.h"

QxrdCenteringPlotWidget::QxrdCenteringPlotWidget(QWidget *parent) :
  QcepPlotWidget(parent),
  m_CenterFinder()
{
}

QxrdCenteringPlotWidget::~QxrdCenteringPlotWidget()
{
}

void QxrdCenteringPlotWidget::initialize(QxrdCenteringPlotWidgetSettingsWPtr settings,
                                         QxrdCenterFinderWPtr cf)
{
  QcepPlotWidget::initialize(settings);
  m_CenterFinder = cf;

  QxrdCenterFinderPtr c(m_CenterFinder);

  if (c) {
    addPlotCommand(QcepPlotCommandPtr(new QxrdSetCenterCommand(this, settings, c)));
  }

  addPlotCommandSpacer();

  addPlotCommand(QcepPlotCommandPtr(new QxrdContextSeparatorCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QxrdEnableRingCommand(this, settings)));
}
