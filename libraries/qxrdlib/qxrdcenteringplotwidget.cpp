#include "qxrdcenteringplotwidget.h"
#include "qcepsetcentercommand.h"
#include "qcepcontextseparatorcommand.h"
#include "qcepenableringcommand.h"
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
                                         QcepCenterFinderWPtr cf)
{
  QcepPlotWidget::initialize(settings);
  m_CenterFinder = cf;

  QcepCenterFinderPtr c(m_CenterFinder);

  if (c) {
    addPlotCommand(QcepPlotCommandPtr(new QcepSetCenterCommand(this, settings, c)));
  }

  addPlotCommandSpacer();

  addPlotCommand(QcepPlotCommandPtr(new QcepContextSeparatorCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepEnableRingCommand(this, settings)));
}
