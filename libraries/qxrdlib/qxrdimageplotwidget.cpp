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
#include "qxrdimageplot.h"
#include "qxrdimageplotwidgetsettings.h"
#include "qxrdscalingsubmenucommand.h"
#include "qxrdcolormapsubmenucommand.h"

QxrdImagePlotWidget::QxrdImagePlotWidget(QWidget *parent) :
  QxrdPlotWidget(parent)
{
}

QxrdImagePlotWidget::~QxrdImagePlotWidget()
{
}

void QxrdImagePlotWidget::initialize(QxrdImagePlotWidgetSettingsWPtr settings)
{
  QxrdPlotWidget::initialize(settings);

  addPlotCommand(QxrdPlotCommandPtr(new QxrdMaskCirclesCommand("Mask Circles", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdMaskPolygonsCommand("Mask Polygons", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdSetCenterCommand("Set Center", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdPowderPointsCommand("Powder Points", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdSliceCommand("Slice", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdMeasureCommand("Measure", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdHistogramCommand("Histogram", this, settings)));
  addPlotCommandSpacer();

  addPlotCommand(QxrdPlotCommandPtr(new QxrdScalingSubmenuCommand("Display", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdColorMapSubmenuCommand("Display", this, settings)));
}
