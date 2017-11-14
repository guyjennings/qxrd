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

QxrdImagePlotWidget::QxrdImagePlotWidget(QWidget *parent) :
  QxrdPlotWidget(parent)
{
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomInCommand("Zoom In")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomOutCommand("Zoom Out")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomAllCommand("Zoom All")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdMaskCirclesCommand("Mask Circles")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdMaskPolygonsCommand("Mask Polygons")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdSetCenterCommand("Set Center")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdPowderPointsCommand("Powder Points")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdSliceCommand("Slice")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdMeasureCommand("Measure")));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdHistogramCommand("Histogram")));
  addPlotCommandSpacer();
}

QxrdImagePlotWidget::~QxrdImagePlotWidget()
{
}
