#include "qxrdcenteringplotwidget.h"
#include "qxrdzoomincommand.h"
#include "qxrdzoomoutcommand.h"
#include "qxrdzoomallcommand.h"
#include "qxrdsetcentercommand.h"
#include "qxrdcontextseparatorcommand.h"
#include "qxrdimageplot.h"

QxrdCenteringPlotWidget::QxrdCenteringPlotWidget(QWidget *parent) :
  QxrdPlotWidget(parent)
{
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomInCommand("Zoom In", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomOutCommand("Zoom Out", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomAllCommand("Zoom All", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdSetCenterCommand("Set Center", this)));
  addPlotCommandSpacer();

  addPlotCommand(QxrdPlotCommandPtr(new QxrdContextSeparatorCommand("Separator", this)));
}

QxrdCenteringPlotWidget::~QxrdCenteringPlotWidget()
{
}
