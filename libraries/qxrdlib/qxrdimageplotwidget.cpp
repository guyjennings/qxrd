#include "qxrdimageplotwidget.h"
#include "qxrdzoomincommand.h"
#include "qxrdzoomoutcommand.h"
#include "qxrdzoomallcommand.h"
#include "qxrdmaskcirclescommand.h"
#include "qxrdmaskpolygonscommand.h"
#include "qxrdsetcentercommand.h"
#include "qxrdpowderpointscommand.h"
#include "qxrdslicecommand.h"
#include "qxrdmeasurecommand.h"
#include "qxrdhistogramcommand.h"
#include "qxrddisplaysubmenucommand.h"

QxrdImagePlotWidget::QxrdImagePlotWidget(QWidget *parent) :
  QxrdPlotWidget(parent)
{
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomInCommand("Zoom In", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomOutCommand("Zoom Out", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomAllCommand("Zoom All", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdMaskCirclesCommand("Mask Circles", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdMaskPolygonsCommand("Mask Polygons", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdSetCenterCommand("Set Center", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdPowderPointsCommand("Powder Points", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdSliceCommand("Slice", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdMeasureCommand("Measure", this)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdHistogramCommand("Histogram", this)));
  addPlotCommandSpacer();

  addPlotCommand(QxrdPlotCommandPtr(new QxrdDisplaySubmenuCommand("Display", this)));
}

QxrdImagePlotWidget::~QxrdImagePlotWidget()
{
}
