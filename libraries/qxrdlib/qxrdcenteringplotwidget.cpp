#include "qxrdcenteringplotwidget.h"
#include "qxrdzoomincommand.h"
#include "qxrdzoomoutcommand.h"
#include "qxrdzoomallcommand.h"
#include "qxrdsetcentercommand.h"

QxrdCenteringPlotWidget::QxrdCenteringPlotWidget(QWidget *parent) :
  QxrdPlotWidget(parent)
{
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomInCommand("Zoom In")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomOutCommand("Zoom Out")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomAllCommand("Zoom All")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdSetCenterCommand("Set Center")));
  addPlotCommandSpacer();
}

QxrdCenteringPlotWidget::~QxrdCenteringPlotWidget()
{
}
